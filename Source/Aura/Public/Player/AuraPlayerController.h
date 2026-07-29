// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
// 定义一个自定义玩家控制器
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY() // 各种预处理
public:
	AAuraPlayerController(); // 该控制器的公有成员
	// 构造函数：类的初始化函数。通常在这里设置一些默认值（默认参数），或者为类创建一些默认的子组件（Subobjects）。
	virtual void PlayerTick(float DeltaTime) override; // 每帧调用一次，DeltaTime 是两帧之间的时间间隔。通常在这里处理一些需要持续更新的逻辑，比如玩家输入、相机控制等。
protected:
	virtual void BeginPlay() override; //当游戏开始、或者这个控制器被生成（Spawn）到世界中时，它会自动触发且仅触发一次。
	// 重写了父类的BeginPlay函数，以便在游戏开始时执行一些特定的逻辑，比如设置输入映射上下文、显示鼠标光标等。
	virtual void SetupInputComponent() override;

// UPROPERTY(...)：虚幻引擎的属性宏。它把这个 C++ 变量注册到引擎的反射系统中，使其能被引擎识别。
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext; // 上下文环境，只要是移动都要经过该环境定义

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction; // 一个输入动作，通常对应IA_Move蓝图，设置值为Vec2D的那个

	void Move(const FInputActionValue& InputActionValue);// 回调函数，获得实际移动数据
	//这是一个非常强大的数据包裹。无论玩家是用手柄摇杆（推多大距离产生多大的浮点数值）还是键盘（按下就是 1，松开就是 0），引擎都会把这些输入数据封装进 InputActionValue 中传给这个函数。
	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	
};
