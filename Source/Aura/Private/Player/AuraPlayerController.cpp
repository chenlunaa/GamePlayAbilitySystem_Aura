// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

// 这行代码告诉虚幻引擎的服务器（Server）：这个 Player Controller 需要进行网络同步（复制）。在多人联机游戏中，服务器需要把控制器的状态、网络 RPC（远程过程调用）正确地分发和同步给对应的客户端。
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

/*
* 鼠标悬停在敌人上
  → CursorTrace 检测到 ThisActor 实现了 IEnemyInterface
  → 调用 HighlightActor()
	→ GetMesh()->SetRenderCustomDepth(true)
	→ GetMesh()->SetCustomDepthStencilValue(250)
  → 后处理体积的 PP_Highlight 材质读取到 Stencil=250
  → 渲染红色轮廓 
 */

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(Damage);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControllerPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControllerPawn->AddMovementInput(Direction);
		
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

// 鼠标移动到人物高亮功能
void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	* 追踪鼠标的状况可能有以下几种情况
	* A. 上个Actor和这个Actor都是空的
	*	-什么都不做
	* B. 上个Actor是空但是这个是有效的
	*	-高亮这个Actor
	* C. 上个Actor是有效的但是这个是空的
	*	-取消高亮上个Actor
	* D. 上个Actor和这个Actor都是有效的，但是这两个Actor不是同一个
	*	-取消高亮上个Actor然后高亮这个Actor
	* E. 上个Actor和这个Actor都是有效的，并且是同一个
	*	-什么都不做
	*/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr) {
			//Case B
			ThisActor->HighlightActor();
		}
		else {
			//Case A
		}
	}
	else // LastActor != nullptr
	{
		if (ThisActor == nullptr) {
			//Case C
			LastActor->UnHighlightActor();
		}
		else //两个Actor都有效
		{
			if (LastActor != ThisActor) {
				//Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else {
				//Case E 什么都不做
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControllerPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControllerPawn->GetActorLocation(), CachedDestination))
			{
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					}
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
	}
}

/*
 * 处理鼠标点击事件时，如果是左键点击则可能是移动指令，判断后处理持续点击时的移动操作
 */
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		
		if (APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	/*
		GetLocalPlayer()：获取当前控制器对应的本地玩家。
		ULocalPlayer::GetSubsystem<...>(...)：获取虚幻5特有的“增强输入本地玩家子系统”。这个子系统是管理整个游戏按键映射的大管家。
		Subsystem->AddMappingContext(AuraContext, 0);：核心一步！把我们在编辑器里配好的 AuraContext 注入到大管家里面。
		参数 0 是优先级（Priority）
	*/
	// 把输入映射上下文添加到玩家子系统中，获取到玩家的增强输入系统，获取到该系统后才能设置各种各样的输入形态
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);// 设置鼠标的样式为操作系统默认的箭头样式。
	}
	
	// 鼠标光标显示的处理
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	/* 虚幻有三种输入模式：仅游戏（GameOnly）、仅UI（UIOnly）、游戏和UI（GameAndUI）。这里使用的是 GameAndUI（混合模式）
	*  设置鼠标不要被锁死在游戏窗口内。这样在窗口化运行时，鼠标可以自由移动到游戏窗口外面。
	*  设置点击游戏画面时不要隐藏鼠标。如果是 FPS 游戏，通常点一下画面鼠标就消失并被游戏“捕获”；但这里设为 false，鼠标会一直可见。
	*  将上述配置好的规则正式应用到当前的 Player Controller 上。
	*/
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 把父类原有的InputComponent转化成增强输入的Component，否则报错
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent); // 转化成这个UEnhancedInputComponent类型，转化失败则程序崩溃，因为默认的输入类型是EnhancedInputComponent.
	// 绑定MoveAction输入事件，当玩家触发这个输入事件时（比如按下 WASD 键或者摇动手柄），就会调用 AAuraPlayerController::Move 函数来处理这个输入事件。
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, & AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, & AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, & AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}
// 获取玩家输入的移动数据InputAxisVector，并根据当前玩家相机的旋转角度GetControlRotation计算出前后左右的移动方向，最后把这些移动输入应用到玩家控制的 Pawn 上。
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation(); // 获取的是当前玩家相机的旋转角度（包含 Pitch 俯仰角、Yaw 偏航角、Roll 翻滚角）。
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // 只保留偏航角（Yaw），把俯仰角（Pitch）和翻滚角（Roll）都设置为 0。这样我们就得到了一个水平旋转的角度，适合用来计算前后左右的移动方向。

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);// 在虚幻引擎中，X 轴代表正前方。这行代码提取出当前角色面对的绝对“前方单位向量”。
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y); //这里和对应的IMC配置相关，这里把WS配置成了Y轴所以表示向前的是获取Y轴的输入。
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}

}


