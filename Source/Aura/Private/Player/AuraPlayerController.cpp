// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// 这行代码告诉虚幻引擎的服务器（Server）：这个 Player Controller 需要进行网络同步（复制）。在多人联机游戏中，服务器需要把控制器的状态、网络 RPC（远程过程调用）正确地分发和同步给对应的客户端。
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
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
	// 把输入映射上下文添加到玩家子系统中
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);// 设置鼠标的样式为操作系统默认的箭头样式。

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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent); // 转化成这个UEnhancedInputComponent类型，转化失败则程序崩溃，因为默认的输入类型是EnhancedInputComponent.
	// 绑定MoveAction输入事件，当玩家触发这个输入事件时（比如按下 WASD 键或者摇动手柄），就会调用 AAuraPlayerController::Move 函数来处理这个输入事件。
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, & AAuraPlayerController::Move);
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
