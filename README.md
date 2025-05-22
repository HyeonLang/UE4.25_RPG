## ▶️ 데모 시연 영상 (이미지 클릭↓)
[![데모 시연 영상](https://github.com/user-attachments/assets/f9f25eb6-61b2-40c4-b3f3-1aafcfe9edc0)](https://youtu.be/nGRqgbBO1Gg)  



# ‘명조’의 전투 구조를 분석하고, 상호작용, 캐릭터 교체 시스템 및 멀티플레이 액션 동기화를 UE4에서 구현한 프로젝트

## 프로젝트 개요 (Project Overview)
| 항목 | 내용 |
|------|------|
| **프로젝트명** | UE4.25_RPG(명조) |
| **개발 기간** | 2024.09 - 2025.03 (6개월) |
| **사용 Tool** | UnrealEngine4, Mysql, Visual Studio, PyCharm |
| **목표** | '명조'의 주요 시스템을 모작하여 **멀티플레이 RPG 전투 시스템과 캐릭터 교체 기능을 구현** |
| **주요 기능** | 캐릭터 교체 시스템, 액션 시스템(일반공격/스킬/궁극기), 멀티플레이 동기화, 상호 작용, 미니맵 및 UI 개발 |
| **기여도** | 2인 개발 (게임플레이 설계 및 구현 전체 담당), (에셋 관련 담당) |


---  
&nbsp; 
![main-압축](https://github.com/user-attachments/assets/6442ada5-a42d-4ccd-a902-6f039d468909)  
## 게임의 목표 (The Goal of The Game)
### **3개의 캐릭터를 조종**하여 **다른 플레이어와** 함께 **적을 처치**하고 **보물상자**를 열어 **아이템을 획득**하자!

&nbsp;
---  
&nbsp;
## 주요 기능 및 기술적 구현 (Key Features & Implementation)
| 기능명 | 설명 및 구현 방식 | 관련 기술 |
|--------|-----------------|------------|
| **캐릭터 교체 시스템** | 플레이어가 3명의 캐릭터를 소환, 동시 소환 및 교체 가능 | `APlayerController::Possess()` | `APlayerController::UnPossess()`
| **액션 시스템** | Unreal GAS를 모티브로 한 일반 공격, 스킬, 궁극기 등의 모듈화된 액션 클래스 | `CActionBase` 상속 | 타겟팅 시스템
| **멀티플레이** | 서버-클라이언트 동기화, RPC, Replicate 사용 | `NetMulticast`, `RepNotify` |
| **상호작용** | 주변 오브젝트와 상호작용 가능, 입력 시 트리거 및 UI 표시 | 트레이스(LineTrace), 서버 실행 구현 |
| **미니맵 및 UI** | `SceneCaptureComponent2D`를 활용하여 실시간 미니맵 렌더링 | UE4 미니맵 시스템 | UI
| **로그인 시스템** | Flask 서버와 MySQL 연동을 통한 로그인 인증 | `HTTP Post`, `GameInstance` |


---  
&nbsp;

## 목차 (인덱스)
0. **[멀티플레이 동기화 - RPC, Replication, RepNotify 사용 패턴](#0-멀티플레이-동기화-Multiplayer-Game-Sync-Techniques)**
1. **[캐릭터 시스템 - 캐릭터의 구조](#1-캐릭터-시스템-player-character-system)**
2. **[캐릭터 교체 시스템 - 동시 제어 교체 구현](#3-캐릭터-교체-시스템-character-swiching-system)**
3. **[액션 시스템 - GAS 스타일 멀티플레이 액션의 구조](#3-액션-시스템-action-system)**
4. **[전투 시스템 - 타겟팅 및 전투 액션](#4-전투-시스템-combat-system)**
5. **[상호작용 시스템 - 액터간 상호작용 실행 방식](#5-상호작용-시스템-interaction-system)**
6. **[적 AI 시스템 - 적의 개별 알고리즘 및 집단적 관리](#6-적-ai-시스템-enemy-ai-system)**
7. **[미니맵 및 UI 시스템 - UI-인게임 간 정보 바인딩 관리](#7-미니맵-및-ui-시스템)**
8. **[데이터 및 에셋 관리 - 데이터 관리 및 접근 흐름](#8-데이터-및-에셋-관리)**
9. **[DB를 활용한 로그인 및 게임 참여 시스템 - DB와의 통신 및 멀티플레이에서의 사용](#9-DB를-활용한-로그인-및-게임-참여-시스템)**
10. **[코스메틱 시스템 - 코스메틱의 종류와 구현](#10-코스메틱-시스템-Cosmetic-System)**
11. **[포트폴리오 영상 & 코드 링크](#11-포트폴리오-영상--코드-링크)**
12. **[기술 이슈 및 해결 기록](#12-기술-이슈-및-해결-기록)**



---  
&nbsp;

  
## 0. 멀티플레이 동기화 (Multiplayer Game Sync Techniques)
### 🛠 UE4.25_RPG 멀티플레이 동기화를 위해 사용한 코딩 패턴
#### **- RPC(Remote Procedure Call) 사용**
  * **캐릭터 교체와 액션, 액터 스폰 등, 멀티플레이 흐름**에 사용된 방법.
  * 동기화 패턴이 아래와 같은 원리를 가지도록 코딩
    ![RPC](https://github.com/user-attachments/assets/f36ed2b9-d430-4436-8ac0-0ab8c01b2483)
    

#### **- 변수 Replication 사용**
  * 체력, 아이템 정보 등의 **변수 동기화**와 액터의 **상태 변화 변수** 동기화를 다루기위해 사용된 방법.
  * 변수 동기화 후 **클라이언트에서 후속 처리**가 필요한 경우 **Replicated Notify**를 사용하여 처리를 한다. [*#동기화 이슈*](#멀티플레이-이슈-클라이언트-동기화-실패)
  * 동기화 패턴이 아래와 같은 원리를 가지도록 코딩 [#*OnRep 이슈*](#onrep-이슈-서버에서-onrep-미동작)  
   ![Rep](https://github.com/user-attachments/assets/53a7dfe8-6c8d-4c21-bb5f-74bedd7b1e6d)

### 목적과 이유
 - 멀티플레이 게임에서 동기화는 필수 요소이며, 설계 없이 진행할 경우 코드가 얽히고 유지보수가 어려워짐.
 - 일정한 코딩 패턴을 정해두어 동기화 문제를 줄이고, 디버깅과 협업이 용이한 구조를 만듦.
    
    
&nbsp;
## 1. 플레이어 캐릭터 시스템 (Player Character System)
### 🛠 플레이어의 캐릭터 사용
  - 플레이어는 각각 3개의 캐릭터(`PlayerCharacter`)를 사용한다.
  - 플레이어의 입력은 동시에 1개의 만 받을 수 있으며 캐릭터 교체 시스템으로 제어된다.
    - `PlayerController`의 제어로 각 캐릭터에 `Possess` / `UnPossess` 및 `SetViewTarget` 카메라 제어 
    - `PlayerController` 소유의 `PlayerCamera`가 캐릭터의 적절한 위치에서 화면을 보여줌 [*#카메라 이슈*](#카메라-이슈-beginplay-시-카메라-빙의-실패)
  
### 🔹 플레이어 캐릭터 구조
  ```
 PlayerController
  |
  ├── PlayerCharacter (Character 상속)
  |   |
  │   ├── AimingComponent         // 타겟팅 관련 처리
  │   ├── StateComponent          // 상태 관리 (Movement 등)
  │   ├── ActionComponent         // 액션 실행 (공격, 점프, 스킬 등)
  │   ├── InteractionComponent    // 상호작용 처리 (아이템 줍기, 보물 상자 열기 등)
  │   ├── AbilityComponent        // GAS 연동 : GAS-AttributeSet을 사용 (HP 등)
  │   │   └── AttributeSet        // 캐릭터의 Attribute 담은 클래스 
  │   ├── ~~IKComponent~~         // IK 제어
  │   └── Weapon                  // 장착 무기
  |
  └── PlayerState
      └── Inventory               // 소유하는 Item 관리
```
 | 컴포넌트 및 소유          | 설명 |
|-------------------|------|
| **`AimingComponent`** | `Action` 실행 시 필요한 타겟팅 처리 (우선순위에 따라 Target 지정) |
| **`StateComponent`**  | `MovementComponent`의 `MaxSpeeds` 관리 및 `StateType` 변화 시 바인딩된 이벤트 처리 |
| **`ActionComponent`** | 스킬, 점프 등의 액션 실행 |
| **InteractionComponent** | 상호 작용 관련 처리 (아이템 줍기, 보물 상자 열기 등) |
| **AbilityComponent** | GAS와 연동하여 `AttributeSet` 사용 |
| &nbsp;&nbsp;└── *AttributeSet* | 캐릭터의 `Attribute`를 정의 및 조정하는 클래스 |
| ~~IKComponent~~     | ~~IK 담당 컴포넌트~~[*#IK 이슈*](#ik-이슈-skeletal-mesh-구조-불일치로-인한-ik-실패) |
| **Weapon**          | 캐릭터의 무기. Damage 처리를 위해 무기의 Collision 및 위치 정보 사용 |
| **Invectory**        | 소유하는 Item을 관리 및 획득, 삭제, 사용 등의 처리 관리 |  
  
[*#캐릭터 외곽선 이슈*](#캐릭터-외곽선-이슈-outline-mesh와-본-mesh의-애니메이션-불일치)


### 💡 구현과 목적
 - 캐릭터 제어가 핵심 기능이므로, 구조를 기능별 컴포넌트로 분리해 가독성과 유지보수성을 높임
 - 캐릭터 교체 시에도 시점을 일정하게 유지하기 위해, 카메라는 캐릭터가 아닌 'PlayerController'에서 생성·관리함
 - 키 입력 또한 다수의 캐릭터에 적용해야 하기 때문에 `PlayerController`에서 받음

&nbsp;
## 2. 캐릭터 교체 시스템 (Character Swiching System) - 전환 기반 멀티캐릭터 제어 시스템
### 🛠 온필드, 오프필드 시스템 구현
- 모작의 **핵심 기능**
- 세명의 캐릭터를 제어. 일반 교체와 협동 공격 교체 등 협동 스킬 구현
- 이벤트 기반을 통한 멀티플레이 동기화
  ![교체](https://github.com/user-attachments/assets/d7ea0256-7c9c-4b64-9bb8-47f6e67ec42b)


### 🔹 캐릭터 교체 방식
| **일반 교체** | **협동 공격 교체** |
|-----|-----|
| ![일반교체gif](https://github.com/user-attachments/assets/7603d5be-95d7-4ea5-b61b-69ac08130309) | ![협주교체gif](https://github.com/user-attachments/assets/c12467b0-1b6b-4181-99e9-dd37f4de55a5) | 
| A → B 캐릭터 교체 시, A가 **즉시** 오프필드, B가 온필드 | A → B 캐릭터 교체 시, A가 **실행 중인 액션이 끝난 후** 오프필드. |
| 동시에 한 캐릭만 필드에 존재한다. |  2명 이상이 온필드로 여러 캐릭터의 액션을 동시에 실행할 수 있다. |
```
void ACPlayerController::UnPossessCharacter(FVector& OutVelocity, EChangeMode InMode)
{   // 캐릭터 UnPossess(교체되어 들어가는) 함수
    ...
    switch (InMode)
	{
	case EChangeMode::None:
      		// 일반 교체시 바로 Hide
		HideCharacter(PlayerCharacter);
		break;
	case EChangeMode::Concerto:
      		// 협동 공격 교체시 액션 종료시 호출되는 Delegate에 Hide 바인딩
		PlayerCharacter->GetActionComponent()->OnActionStopped.AddDynamic(this, &ACPlayerController::OnActionStopped_HideCharacter);
		break;
	default:
		break;
	}
    ...
    // 캐릭터 멀티플레이 동기화 (서버 RPC) 및 쿨타임 등 수행
}
```

### 🔹 멀티플레이 환경 적용
- 로컬 클라이언트에서 `PossessCharacter()` 실행 후, 서버에서 RPC 실행
- ServerRPC를 활용하여 기능 수행 및 변수를 변경하고 `RepNotify`를 활용하여 클라이언트에 상태 동기화  
- `Delegate.Broadcast()`로 협주 교체 후 UnpossessCharacter() 실행

### 💡 구현과 목적
 - 일반 캐릭터 교체는 Possess → Unpossess 순서가 보장되지만, 협동 공격 교체는 이 순서가 보장되지 않음.
 - 따라서 Possess를 먼저 수행하고, Unpossess 관련 처리는 액션 종료 후 실행되도록 delegate에 바인딩해 예약 처리함.
 - 협동 공격 교체시 교체하여 나오는 캐릭터가 겹치는 문제의 해결을 위해
  - 카메라의 RightVector 방향으로 일정거리 이동하여 나타나도록 함.

---  
&nbsp;


## 3. 액션 시스템 (Action System)
### 🛠 캐릭터 액션 및 네트워크 동기화
- 상태 변경 관련된 버그에 특히 신경을 많이 썼습니다.(액션 캔슬 전용 애님노티파이 적용 등)
- 액션 도중 캐릭터 교체와 같은 예외적인 상황을 대비하도록 설계되었습니다.
- 서버 RPC와 네트워크 복제를 활용하여 액션의 상태와 동작이 모든 유저에게 동일하도록 보장합니다.
![동기화](https://github.com/user-attachments/assets/301c57d7-21d9-4d5b-851d-d97b778c8f22)

### 🔹 액션 클래스 구조
```
CActionBase
│
├── CAction
│   ├── CAction_NormalAttack (일반공격)
│   ├── CAction_ResonanceLiberation (궁극기)
│   ├── CAction_ResonanceSkill (스킬)
│   ├── CAction_CancelLoop
│
└── CNPCAction

// C++ 코드를 상속받아 Unreal Blueprint 클래스로 다변화
```

### 🔹 액션과 Unreal GAS  
- 본 게임의 액션은 GAS의 **Gameplay Ability**를 참고하여 제작하여 다음과 같은 특징을 공유한다
    - **Action-Start(GAS-Activate) --- Action-Stop(GAS-End)** 구조를 가짐
    - CanStart(GAS-CanActivate), Cooldown-Manager(GAS-Cooldown)을 사용하여 액션의 실행을 관리
    - ActionName(GAS-Tag)으로 **Name 기반 액션 호출**
    - FGameplayTagContainer::**ActiveGameplayTags(GAS-Ability Tags)의 사용**으로 액션의 **조건 제어, 중첩 제한** 등을 관리  
      ![tags](https://github.com/user-attachments/assets/7b06cb01-75fe-4597-a1cc-bba001555f1f)
      
### 🔹 액션 실행 흐름
```cpp
Character->ActionComponent->StartActionByName() 액션 호출
if (CanStart())
{
  StartAction(); // 액션 실행
}

// AnimNotify 혹은 몽타주 끊김 이벤트에서
---> StopAction();  // 액션 종료 호출
```
![액션](https://github.com/user-attachments/assets/0cc4a9a1-23cf-4b6e-a4f3-531fab38760b)  
1. `ActionComponent`에서 `CanStart()`를 호출하여 실행 가능 여부 확인  
2. `StartAction()` 실행 시 **몽타주 실행, Gamepaly tag 추가, 쿨타임 실행, 이동 입력 제어** 적용
3. 애니메이션 몽타주 등으로 `StopAction()` 호출
4. `StopAction()`을 통해 액션 종료 후 초기 상태 복구
      
&nbsp;
- **루프 액션 (키다운 공격)의 흐름**
  - (일반적) 키 누름 유지로 Loop하는 공격은 키를 떼면 공격 종료 (`Loop Action(루프 액션)`)
  - (특수 상황) `Loop Action(루프 액션)` 실행 중 캐릭터 교체 시 **키를 떼는 동작**이 **교체된 캐릭터의 입력**으로 넘어가므로 공격 종료 입력불가
  - 따라서 `CancelLoop Action(취소 액션)` 실행으로 **루프 액션의 중단**을 호출
  - 여기서 루프 중단이란 **몽타주 Loop 섹션을 break** 하는 것이다. (AnimNotify로 자연스럽게 `StopAction` 된다.) 
   ![루프액션종료-교체시](https://github.com/user-attachments/assets/70086e0c-00b2-4000-af90-175251a1d999)


### 🔹 서버-클라이언트 동기화
- 서버에서 `ActionComponent`를 호출하여 액션 실행  
- RPC 함수(`NetMulticast`, `RepNotify`)를 사용하여 클라이언트에서도 실행  
- Replicated `RepData.bIsRunning` 값을 활용하여 클라이언트에서 `RepNotify()` 함수 호출  

### 🔹 액션 실행 시 버그 방지
- 모든 몽타주(애니메이션)에 `StopAction AnimNotify` 추가  
- `Montage Cancel` 전용 함수(`StopAction()` 호출 후 애니메이션 중단) 사용
- 액션 실행시 필요한 상태 변화(`CharacterMovement` State 관련)에 대해 원상 복귀를 엄격히 시행  [*#액션 이슈*](#액션-이슈-공중-몽타주-동작-실패)

  
### 💡 구현과 목적
- GAS는 검증된 효율적인 시스템이므로 참고하는 것이 프로젝트의 발전을 위해 좋다고 생각. 
- 액션을 서버에서 먼저 실행하면 입력 반응이 느려 조작감이 떨어짐.
 - 조작감이 중요한 부분(몽타주 등)은 클라이언트가 먼저 실행하게 하고 다만 몽수치 변화나 판정 등 핵심 로직은 서버에서 처리함.
- 루프 액션의 종료를 액션으로 만든 것의 경우 루프 액션의 종료 호출도 사용자가 입력하는 실행이기 때문에 액션으로 구현.


---  
&nbsp;


## 4. 전투 시스템 (Combat System)
![전투_압축](https://github.com/user-attachments/assets/3a110907-bb21-4877-94cb-2a7aa97a9c75)  
### 🛠 전투의 흐름
- 명조와 동일하게 **타겟팅**에 따라 액션 방향의 우선 순위를 실행할 수 있도록 함.
  * **타켓팅**은 일정 범위 안에 있는 타겟을 모두 `Trace`한 뒤, 카메라가 바라보는 방향과 가까운 액터를 가중치로하여 **우선순위큐**로 가중치가 가장 높은 액터 하나를 지정하여 실행.
  * 플레이어의 캐릭터는 액션 사용시 **타겟**의 방향을 **바라보고 이동**하여 공격.  
  ![타켓팅](https://github.com/user-attachments/assets/afea5203-fe22-4303-9856-7eae5b09baef) ![타겟팅gif](https://github.com/user-attachments/assets/59ffb2e6-57a0-445f-b72d-d5ea59601580)
- 멀티플레이 환경에서 **다수의 플레이어**가 **동시**에 전투 가능


- 적 캐릭터는 AI 기능을 활용하여 구현.
- 3명의 플레이어블 캐릭터에게 고유한 일반공격, 스킬, 궁극기를 구현.
  ![궁극기gif](https://github.com/user-attachments/assets/cb5079ec-4055-4e84-9427-4e50f593ac4a)
   (궁극기 컷씬 모습) [#*카메라 연출 이슈*](#카메라-연출-이슈-캐릭터-중심-시네마틱-카메라-구현)


### 🔹 대쉬, 점프, 스프린트
- 모든 플레이어 캐릭터가 `Dash()`, `Jump()`, `Sprint()` 공통 기능을 보유  
- 상속 구조로 구성하여 **블루프린트에서 모듈화 가능**  
  ![대쉬점프gif](https://github.com/user-attachments/assets/a7b5d747-40ce-4ee1-9aa6-0d8049004c21)


### 🔹 공격 시스템
| 공격 유형 | 구현 방식 | 관련 기술 |
|---------|--------|---------|
| **일반 공격** | `AimingComponent`에서 적 타겟팅 후 방향과 위치 조정 | `ActionComponent` |
| **콤보 시스템** | `bCanCombo` 변수를 활용하여 연속 공격 가능 | `Notify` 활용 |
| **스킬 시스템** | `CCooldownManager`를 활용하여 쿨타임 관리 | `UObject` 상속 |
| **궁극기 시스템** | 전용 카메라 액션 적용 (`SetViewTarget`) | `AnimNotify` |

#### 콤보 시스템 구현
 - `ActionComponent`에서 `bCanCombo`변수를 체크
 - **`bCanCombo` + 키입력**시 콤보 실행
 -  미리 지정된 **다음 콤보 액션**을 실행. (`Action`마다 있는 `Action->NextComboActionName`을 사전에 데이터로 지정)
 -  키입력 시 받는 **BaseAction이 실행할 콤보의 ActionName을 변경**하면서 콤보 실행
 ```
bool UCActionComponent::StartActionByName(AActor* Instigator, FName ActionName, bool bBaseAction)
{
	...
	// NextCombo
	if (Action->GetCanCombo())
	{
		...
		Action->SetCanCombo(false); // 현재 액션의 콤보 상태를 false로 돌려줌
		Action->CurrentComboActionName = CurrentComboAction->NextComboActionName; // 액션의 현재 콤보를 다음 콤보로 옮김

		StartActionByName(Instigator, Action->CurrentComboActionName, false); // **옮긴 콤보 액션을 재귀적으로 StartAction실행**
		
		if (GetActionByName(Action->CurrentComboActionName)->NextComboActionName == "None")
		{	// 콤보가 끝나면 콤보를 원상태로 복귀
			Action->CurrentComboActionName = ActionName;
		}
		return false; // 직접 액션을 Start하지 않았으므로 false
	}
	...
	// 일반적인 StartAction 실행 코드
}
 ```

### 💡 구현과 목적
- 원작 명조와 같은 게임의 난이도는 다양한 사람들이 즐기기 좋게 설계되어있고 이것에는 타겟팅이 핵심 기능.
- 스킬 입력은 `PlayerController`, 실행은 `ActionComponent`에서 이루어져 콤보 입력 제어가 복잡함.
- 또한 `bCanCombo`에 따라 입력을 분기해야 하지만, 입력과 실행 위치가 분리되어 있어 구현이 난해했음.
 - 따라서 키 입력을 하나의 베이스 `Action`으로 통일하여 처리.
 - 해당 `Action` 내부에 연계 가능한 `Action`들의 정보를 포함시켜 콤보 입력과 실행 흐름을 `ActionComponent`에서 일관되게 제어할 수 있도록 설계함.
 
---  
&nbsp;


## 5. 상호작용 시스템 (Interaction System)
### 🛠 동작 방식
- 캐릭터의 `InteractionComponent`에서 담당
- `UCInteractionInterface` 인터페이스를 상속받아 `Interact()` 함수 구현
  - 상호작용시 발생할 이벤트 구현
- `F` 키 입력 시 `InteractionComponent`에서 Trace하여 주변의 상호작용 가능한 액터 탐색  
- 서버 RPC 함수로 실행하여 모든 클라이언트에서 일관된 상호작용 보장 [RPC 패턴 사용](#0-멀티플레이-동기화-Multiplayer-Game-Sync-Techniques)
![상호작용](https://github.com/user-attachments/assets/9f4a8027-4bf9-421f-a4b3-1422a1654865)  

| **보물상자 (Chest)** | **아이템 (ItemBase)** |
|-------------|-----|
| ![chest](https://github.com/user-attachments/assets/cc059c4f-52d8-43c0-a602-62cae999471e)  | ![아이템획득](https://github.com/user-attachments/assets/12515306-ee6e-4c31-b58f-dd25b1236b37) |
| 상호작용 시 보물상자가 열리며 아이템 스폰 | 플레이어 `Inventory`에 아이템 추가 |

- 인벤토리의 아이템 갯수는 변수 Replication 사용 [Replication 패턴 사용](#0-멀티플레이-동기화-Multiplayer-Game-Sync-Techniques) [*#인벤토리 이슈*](#인벤토리-이슈-tmap-및-uobject-리플리케이션-문제)  
  
### 💡 구현과 목적
- 상호작용은 단일 플레이어의 반응에 그치지 않고, 모든 플레이어에게 동일한 결과가 보여져야 함.
 - 서버에서 실행한 후, 클라이언트로 전달, 복제(Broadcast)되도록 설계.
- 모든 상호작용 액터는 `UCInteractionInterface`를 상속받도록 하여, 기능 구현 및 구분이 쉽게 되도록 구조화함.

---  
&nbsp;

## 6. 적 AI 시스템 (Enemy AI System)
### 🛠 언리얼 AI 및 리스폰 기능
- `EnemyCharacter` 클래스로 구성, `NPCActionComponent`에서 공격 실행  
- `AIController` 및 `Behavior Tree`를 활용하여 AI 상태 관리
  ![bihavior](https://github.com/user-attachments/assets/7b3c465f-662c-443d-a350-7843daa92f1b)


- **`EnemySpawner`**를 활용하여 적 집단의 스폰 및 자동 리스폰 구현
  - `EnemySpawner` 중심과의 거리에 따라 적 집단의 어그로를 조절  
     ![적](https://github.com/user-attachments/assets/0b129081-f7de-4c4c-b40a-44179dd6e1df)
  
  - `EnemySpawner` 하위의 **`SpawnTargetPoint`**에서 적이 생성되며 대기 및 리스폰 위치 관리
    - 적 모두 처치 후 리스폰 시간 UI 출력  
      ![리스폰](https://github.com/user-attachments/assets/d1415971-e156-432f-aaba-2fb98d47fa9a)

### 💡 구현과 목적
 - 적 배치와 전투 흐름을 설계할 때, 개별  행동과 집단 시스템 로직이 섞이면 유지보수와 확장성이 떨어짐.
 - 이동, 공격, 아이템 드롭 같은 액션 부분은 개별  단위로 관리.
 - 어그로, 리스폰, 보물상자 해금 등 시스템적인 집단 단위 처리는 `Spawner`-`SpawnTargetPoint` 구조로 관리해 설계를 단순화함.


---  
&nbsp;


## 7. 미니맵 및 UI 시스템
### 🛠 실시간 미니맵, UI와 인벤토리의 동적 로딩
- `WB_MainHUD`에서 스킬, 체력, 캐릭터 교체 UI 관리
  ![UI](https://github.com/user-attachments/assets/58f4cf52-ff42-4120-984f-9dd1442b553b)
  - 액터의 정보 생성 생명주기와 UI 생성 생명주기가 맞지 않을 수 있어 Null참조 주의 [*#UI 이슈*](#ui-이슈-ui와-데이터-생성-타이밍-불일치)
    ![UI정보과정](https://github.com/user-attachments/assets/18e1b3c1-0722-4453-9a60-29713b8fdee5)
  

- 2개의 `SceneCaptureComponent2D`를(맵의 **깊이 + 색**) 활용한 실시간 미니맵 구현 [#*미니맵 이슈*](#미니맵-이슈-rendertarget-공유-및-렌더링-문제)
  ![Minimap](https://github.com/user-attachments/assets/b51a393c-fd4e-46f3-9922-64ba2dd53290)
  
- **Inventory UI 에서 포션과 같은 소비아이템 사용가능 (아이템의 효과 생성)**  
  ![물약-압축](https://github.com/user-attachments/assets/0f48bec8-d892-46c4-9338-47f518629dd1)

  
- `Soft References`를 활용하여 UI 최적화 및 데이터 로딩 감소 [*#Soft References 이슈*](#icon-이슈-softreference-이미지-로딩-실패)  


### 💡 구현과 목적
- UI는 공통 로직과 Tick이 중요한 부분만 C++로 구현하고, 나머지는 Blueprint로 구현.
 - 개발함에 있어서 시각적 요소가 많고 디자인과 밀접하게 연관된 UI는 Blueprint가 빠르게 반복 작업을 하기 적합하다고 생각함.  

---  
&nbsp;

## 8. 데이터 및 에셋 관리
### 🛠 데이터 테이블을 통한 관리
- 캐릭터의 `GameAblitySystem`의 **AttributeSet을 위한 Attribute(HP 등의 수치)** 데이터 테이블
  - `AbliltySystemComponent` 내의 `AttributeSet`에 사용
  ![Attribute](https://github.com/user-attachments/assets/4d75a88c-99b2-4d3c-94f1-b058029408e3)  

- **아이템의 각 정보**를 담은 ItemInfo 및 ConsumableItemInfo(소비) 데이터 테이블
  ![Item](https://github.com/user-attachments/assets/baf59ed0-ed0e-460c-bcd9-1995738c2d2d)




### 🛠 데이터 에셋을 통한 관리
- 캐릭터의 각 **액션(전투 스킬 등)**마다 사용하는 사거리 등 **수치 데이터** 및 **몽타주, 이펙트 등의 에셋**을 관리하는 데이터 에셋   
  ![ActionDataAsset](https://github.com/user-attachments/assets/21b8c75a-6cac-4fc8-a7f3-eb76c096a1ce)


### 목적과 이유
- 공통적으로 사용하는 대량의 데이터를 다루는 경우. (ex:아이템 정보)
 - 데이터 테이블의 사용이 데이터 사용, 수정에 용이.
- 전용으로 사용되는 데이터 집합의 경우. (ex:`Action`)
 - 각각 데이터 에셋을 두어 필요한 데이터 집합을 구분하면 데이터 사용에 대한 접근성이 올라가고 수정이 용이.

---  
&nbsp;


## 9. DB를 활용한 로그인 및 게임 참여 시스템
### 🛠 DB 시스템을 활용한 로그인 인증
- `Flask` 서버와 `MySQL`을 활용하여 HTTP `POST` 방식으로 유저 데이터 인증  [*#DB 이슈*](#db-이슈-간헐적-접속-실패)
- `GameInstance`에 `DBManager`를 생성하여 DB연결 및 요청, 응답과 유저 데이터 관리
 ```
void UCDBManager::RequestLogin(const FString& Username, const FString& Password)
{
	// 로그인 데이터 생성 및 중복 방지
	...
  	Request->OnProcessRequestComplete().BindUObject(this, &UCDBManager::OnLoginResponse); // 응답 Delegate 바인딩
 	Request->ProcessRequest(); // 로그인 요청
	...
}

void UCDBManager::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 로그인 요청 완료 및 응답 받은 데이터 설정
 	...
  	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    	{
		OnLoginSuccessEvent.Broadcast(ResponseString);  // 로그인 성공 시 사용자 데이터 전달
	}
  	...
}
 ```
  
#### 📌 클라이언트-서버-데이터베이스 요청 흐름
| 클라이언트       | 서버 (Backend)                  | 데이터베이스 (DB)            |
|------------------|----------------------------------|------------------------------|
| POST 요청 전송 🡆 |                                 |                              |
|                  |    요청을 처리,DB에 쿼리 실행 🡆  |                              |
|                  |                                  | 🡆 쿼리 실행 및 결과 반환    |
|                  |     🡄 DB 응답을 받아 처리        |                             |
| 🡄 서버 응답 반환 |                                  |                             |

![image](https://github.com/user-attachments/assets/2d3967e3-ffdb-403a-bb52-e0e04ca22a40)
- 로그인 화면에서 보이듯이 **클라이언트 각각의 정보 구분**

---  
&nbsp;
## 10-코스메틱-시스템-Cosmetic-System  
#### 🔹 Ghost Trail
  - `Character`의 `Skeletal`을 Copy Pose 후 Trail Material로 Ghost Trail 액터 생성
  - 몽타주 실행시 필요한 구간에 GhostTrail 생성 AnimNotify 실행  
  ![Trail](https://github.com/user-attachments/assets/d1d840ac-f6d6-4898-bf53-ed06fb67aa2c)

#### 🔹 Weapon 세팅 
  - 원작과 같이 액션 사용, 대기 모션 등 필요할 때만 무기가 등장하도록 구현  
  ![WeaponSet](https://github.com/user-attachments/assets/db46c872-9c68-434b-a78b-cb1a017f87cd)

#### 🔹 Weapon Trail
  - 반복하는 Weapon Trail Animation을 생성, 사용해서 Weapon Trail 구현
  - Trail 필요에 따라 Weapon Trail Animation ON/OFF  
  ![TrailAttack](https://github.com/user-attachments/assets/288023b1-a41a-4101-a75e-e9305085a84d)


---  
&nbsp;
## 11. 포트폴리오 영상 & 코드 링크
- 📌 **[포트폴리오 영상 보기](https://youtu.be/xxxxxxx](https://youtu.be/nGRqgbBO1Gg))**  
- 📌 **[GitHub 소스코드 확인](https://github.com/HyeonLang/UE4.25_RPG/tree/main/Source)**  
- 📌 **[게임 데모 다운로드](https://github.com/HyeonLang/UE4.25_RPG)**


---  
&nbsp;
## 12. 기술 이슈 및 해결 기록  
&nbsp;
### 멀티플레이 이슈: 클라이언트 동기화 실패 
[🔙복귀](#0-멀티플레이-동기화-multiplayer-game-sync-techniques)
* **상태**: ✅ 해결됨
* **원인**: 서버 실행 중 클라이언트 접속 시 자동 리플리케이션 누락
* **해결**: 클라이언트 초기화 시 RPC 함수 등으로 수동 동기화 트리거 처리
* **고찰**: 상태 초기화 및 재동기화 설계 필요  

&nbsp;
### 카메라 이슈: BeginPlay 시 카메라 빙의 실패
[🔙복귀](#1-플레이어-캐릭터-시스템-player-character-system)
* **상태**: ✅ 해결됨
* **원인**: `bAutoManageActiveCameraTarget`로 인한 카메라 자동 빙의 충돌
* **해결**: 해당 옵션 비활성화 후 수동 빙의 처리
* **고찰**: 카메라 제어의 명확한 주체 설정 중요  
  
&nbsp;
### IK 이슈: Skeletal Mesh 구조 불일치로 인한 IK 실패
[🔙복귀](#1-플레이어-캐릭터-시스템-player-character-system)
* **상태**: ❌ 해결 불가 (보류)
* **원인**: 캐릭터 발 본 방향이 언리얼 기본 마네킹과 반대 → IK 적용 시 발 위치가 머리 위로 이동
* **해결**: 언리얼4 구조와 호환되지 않아 구조적으로 IK 적용 불가
* **고찰**: UE5로 리팩토링 후 표준 본 구조 적용 예정  

&nbsp;
[🔙복귀](#1-플레이어-캐릭터-시스템-player-character-system)
### 캐릭터 외곽선 이슈: Outline Mesh와 본 Mesh의 애니메이션 불일치 

* **상태**: ❌ 해결 불가
* **원인**: Outline용 MeshComponent에 애니메이션 적용 시 위치 어긋남 발생
* **해결**: 외곽선 표현 방식 폐기
* **고찰**: 포스트 프로세스 기반 외곽선 처리로 전환 고려  

&nbsp;
### 인벤토리 이슈: TMap 및 UObject 리플리케이션 문제
[🔙복귀](#5-상호작용-시스템-interaction-system)
* **상태**: ✅ 해결됨
* **원인**: 인벤토리 아이템 정보 저장을 위한 `TMap`은 Replicate 불가, `UObject`는 RPC 미지원
* **해결**: `Rep_Struct` 사용, `NetMulticast` 대신 `OnRep` 기반 동기화
* **고찰**: 서버 응답 지연 대비 Interaction 쿨타임 1초 설정  

&nbsp;
### DB 이슈: 간헐적 접속 실패
[🔙복귀](#9-db를-활용한-로그인-및-게임-참여-시스템)
* **상태**: ✅ 해결됨
* **원인**: MySQL C++ 커넥터의 연결 불안정 문제
* **해결**: Flask 중계 서버 구축, HTTP 방식으로 MySQL 연결 처리
* **고찰**: 안정성과 실시간성을 위한 REST 기반 아키텍처 설계 고려  

&nbsp;
### UI 이슈: UI와 데이터 생성 타이밍 불일치
[🔙복귀](#7-미니맵-및-ui-시스템)
* **상태**: ✅ 해결됨
* **원인**: 데이터를 참조하는 UI가 데이터 생성보다 먼저 실행되어 null 접근 발생
* **해결**: UI를 순회하며 데이터 연결을 검사하는 로직 및 실패 시 데이터 재요청 로직 추가
* **고찰**: UI-데이터 의존성 파악 및 타이밍 제어 중요  

&nbsp;
### 미니맵 이슈: RenderTarget 공유 및 렌더링 문제
[🔙복귀](#7-미니맵-및-ui-시스템)
* **상태**: ✅ 해결됨
* **원인**: 머티리얼 에셋 공유로 `TextureRenderTarget2D`을 마지막 클라이언트가 덮어씀
* **해결**: `NewObject`로 RenderTarget 생성, 복수의 CaptureSource 조합 사용
* **고찰**: 에셋 공유 최소화, 런타임 객체 생성 선호  

&nbsp;
### 액션 이슈: 공중 몽타주 동작 실패
[🔙복귀](#3-액션-시스템-action-system)
* **상태**: ✅ 해결됨
* **원인**: 공중에서 몽타주 루트모션 적용 실패
* **해결**: 실행 시 Flying 설정해야 공중 몽타주가 적용됨 → 종료 후 상태 복원
* **고찰**: 상태 기반 액션 로직 구현 중요    

&nbsp;
### Blueprint 이슈: NativeEvent와 TArray 레퍼런스 호환성
[🔙복귀](#3-액션-시스템-action-system)
* **상태**: ✅ 해결됨
* **원인**: `TArray&`는 `BlueprintNativeEvent`함수에서 매개변수로 사용불가
* **해결**: `const TArray&` 또는 복사 방식으로 수정
* **고찰**: BP ↔ C++ 함수 시그니처 제약 사항 숙지 필요   

&nbsp;
### UObject Tick 이슈: 오브젝트 기반 Tick 비효율
[🔙복귀](#3-액션-시스템-action-system)
* **상태**: ✅ 해결됨
* **원인**: `UObject`는 Tick 미지원으로 지속 처리 어려움
* **해결**: Owner `AActor` Tick 내에서 UObject의 커스텀 Tick 호출
* **고찰**: UObject vs AActor의 라이프사이클과 구조 차이 이해 중요  

&nbsp;
### Icon 이슈: SoftReference 이미지 로딩 실패
[🔙복귀](#7-미니맵-및-ui-시스템)
* **상태**: ✅ 해결됨
* **원인**: `SetBrushFromSoftTexture`에 SoftReference 직접 사용 시 로딩 안됨
* **해결**: 블루프린트에서 비동기 로딩 후 이미지 적용
* **고찰**: SoftReference 사용 시 로딩 유의  

&nbsp;
### OnRep 이슈: 서버에서 OnRep 미동작 [복귀](#0-멀티플레이-동기화-Multiplayer-Game-Sync-Techniques)
[🔙복귀](#0-멀티플레이-동기화-multiplayer-game-sync-techniques)
* **상태**: ✅ 해결됨
* **원인**: `OnRep` 함수는 클라이언트에서만 자동 실행
* **해결**: 서버에서 수동으로 `OnRep` 함수 호출
* **고찰**: OnRep의 동작 범위 정확히 이해해야 혼란 방지    

&nbsp;
### 카메라 연출 이슈: 캐릭터 중심 시네마틱 카메라 구현
[🔙복귀](#4-전투-시스템-combat-system)
* **상태**: ✅ 해결됨
* **원인**: 레벨 시퀀서가 월드 기준 Transform으로 캐릭터 기준 연출 실패
* **해결**: (0, 0, 0) 좌표를 기준으로 레벨 시퀀서로 구현. 그후 커브 추출 후 캐릭터 위치를 더해 별개의 카메라에 타임라인 적용
* **고찰**: 월드/로컬 좌표 구분 및 커브 기반 카메라 제어 숙지 필요  
