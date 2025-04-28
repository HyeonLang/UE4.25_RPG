## ▶️ 데모 시연 영상 (이미지 클릭↓)
[![데모 시연 영상](https://github.com/user-attachments/assets/8b280d82-3860-4e9a-a907-525951274d3f)](https://youtu.be/nGRqgbBO1Gg)


# 게임 `명조`를 모작한 멀티플레이 기반 게임

## 프로젝트 개요 (Project Overview)
| 항목 | 내용 |
|------|------|
| **프로젝트명** | UE4.25_RPG |
| **개발 기간** | 2024.09 - 2025.03 (6개월) |
| **사용 Tool** | UnrealEngine4, Mysql, Visual Studio, PyCharm |
| **목표** | '명조'의 주요 시스템을 모작하여 **멀티플레이 RPG 전투 시스템과 캐릭터 교체 기능을 구현** |
| **주요 기능** | 캐릭터 교체 시스템, 액션 시스템(일반공격/스킬/궁극기), 멀티플레이 동기화, 상호 작용, 미니맵 및 UI 개발 |
| **기여도** | 1인 개발 (설계부터 구현까지 전체 담당) |

---

## 주요 기능 및 기술적 구현 (Key Features & Implementation)
| 기능명 | 설명 및 구현 방식 | 관련 기술 |
|--------|-----------------|------------|
| **캐릭터 교체 시스템** | 플레이어가 3명의 캐릭터를 소환, 동시 소환 및 교체 가능 | `APlayerController::Possess()` | `APlayerController::UnPossess()`
| **액션 시스템** | 일반 공격, 스킬, 궁극기 등의 액션을 모듈화 | `CActionBase` 상속 | 타켓팅 시스템
| **멀티플레이** | 서버-클라이언트 동기화, RPC 사용 | `NetMulticast`, `RepNotify` |
| **미니맵 및 UI** | `SceneCaptureComponent2D`를 활용하여 실시간 미니맵 렌더링 | UE4 미니맵 시스템 | UI
| **로그인 시스템** | Flask 서버와 MySQL 연동을 통한 로그인 인증 | `HTTP Post`, `GameInstance` |

---

## 목차 (인덱스)
0. **[멀티플레이 동기화](#0-멀티플레이-동기화-Multiplayer-Game-Sync-Techniques)**
1. **[캐릭터 교체 시스템](#1-캐릭터-교체-시스템-character-switching-system)**
2. **[액션 시스템](#2-액션-시스템-action-system)**
3. **[전투 시스템](#3-전투-시스템-combat-system)**
4. **[상호작용 시스템](#4-상호작용-시스템-interaction-system)**
5. **[적 AI 시스템](#5-적-ai-시스템-enemy-ai-system)**
6. **[미니맵 및 UI 시스템](#6-미니맵-및-ui-시스템)**
7. **[데이터 및 에셋 관리](#7-데이터-및-에셋-관리)**
8. **[로그인 및 게임 참여 시스템](#8-로그인-및-게임-참여-시스템)**
9. **[포트폴리오 영상 & 코드 링크](#9-포트폴리오-영상--코드-링크)**  

  
## 0. 멀티플레이 동기화 (Multiplayer Game Sync Techniques)
### 🛠 UE4.25_RPG 멀티플레이 동기화를 위해 사용한 코딩 패턴
- RPC(Remote Procedure Call) 사용
  * **캐릭터 교체와 액션, 액터 스폰 등, 멀티플레이 흐름**에 사용된 방법.
  * 동기화 패턴이 아래와 같은 원리를 가지도록 코딩
    ![RPC](https://github.com/user-attachments/assets/f36ed2b9-d430-4436-8ac0-0ab8c01b2483)
    

- 변수 Replication 사용
  * 체력, 아이템 정보 등의 **변수 동기화**와 액터의 **상태 변화 변수** 동기화를 다루기위해 사용된 방법.
  * 변수 동기화 후 **클라이언트에서 후속 처리**가 필요한 경우 **Replicated Notify**를 사용하여 처리를 한다.
  * 동기화 패턴이 아래와 같은 원리를 가지도록 코딩
   ![Rep](https://github.com/user-attachments/assets/53a7dfe8-6c8d-4c21-bb5f-74bedd7b1e6d)



## 1. 캐릭터 교체 시스템 (Character Switching System)
### 🛠 온필드, 오프필드 시스템 구현
- 모작의 핵심 기능
- 세명의 캐릭터를 제어. 일반 교체와 협주 교체 등 협동 스킬 구현
- 이벤트 기반을 통한 멀티플레이 동기화
- ![image](https://github.com/user-attachments/assets/1145a362-fdf9-4abc-b508-d9d9d166cbaa)

### 🔹 캐릭터 교체 방식
| 유형 | 설명 |
|-----|-----|
| **일반 교체** | A → B 캐릭터 교체 시, A가 즉시 오프필드, B가 온필드 |
| **협주 교체** | A → B 캐릭터 교체 시, A가 실행 중인 액션이 끝난 후 오프필드 |

### 🔹 멀티플레이 환경 적용
- 로컬 클라이언트에서 `PossessCharacter()` 실행 후, 서버에서 RPC 실행  
- `RepNotify`를 활용하여 클라이언트에 상태 동기화  
- `Delegate.Broadcast()`로 협주 교체 후 UnpossessCharacter() 실행

---

## 2. 액션 시스템 (Action System)
### 🛠 캐릭터 교체 액션 및 네트워크 동기화
- 상태 변경 관련된 버그에 특히 신경을 많이 썼습니다.(액션 캔슬 전용 애님노티파이 적용)
- 액션 도중 캐릭터 교체와 같은 예외적인 상황을 대비하도록 설계되었습니다.
- 서버 RPC와 네트워크 복제를 활용하여 액션의 상태와 동작이 모든 유저에게 동일하도록 보장합니다.
![image](https://github.com/user-attachments/assets/051b4bbd-9711-4cc0-8a60-5aa2cd7aa22a)

🔹 액션 클래스 구조
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

### 🔹 액션 실행 흐름
```cpp
Character->ActionComponent->StartActionByName() 액션 호출
if (CanStart())
{
  StartAction(); // 액션 실행
  StopAction();  // 액션 종료
}
```
![image](https://github.com/user-attachments/assets/91ecc691-97c0-4a3e-baa4-f31cb3949c11)  
1. `ActionComponent`에서 `CanStart()`를 호출하여 실행 가능 여부 확인  
2. `StartAction()` 실행 시 **몽타주 실행, Gamepaly tag 추가, 쿨타임 실행, 이동 입력 제어** 적용
3. 애니메이션 몽타주 등으로 `StopAction()` 호출
4. `StopAction()`을 통해 액션 종료 후 초기 상태 복구

 

### 🔹 서버-클라이언트 동기화
- 서버에서 `ActionComponent`를 호출하여 액션 실행  
- RPC 함수(`NetMulticast`, `RepNotify`)를 사용하여 클라이언트에서도 실행  
- Replicated `RepData.bIsRunning` 값을 활용하여 클라이언트에서 `RepNotify()` 함수 호출  

### 🔹 액션 실행 시 버그 방지
- 모든 몽타주(애니메이션)에 `StopAction AnimNotify` 추가  
- `Montage Cancel` 전용 함수(`StopAction()` 호출 후 애니메이션 중단) 사용  


---

## 3. 전투 시스템 (Combat System)
### 🛠 전투의 흐름
- 명조와 동일하게 **타겟팅**에 따라 액션 방향의 우선 순위를 실행할 수 있도록 하였습니다.
  * **타켓팅**은 카메라가 바라보는 방향과 가까운 액터를 가중치로하여 **우선순위큐**로 액터 하나를 지정하여 실행합니다.
  * 플레이어의 캐릭터는 액션 사용시 **타겟**의 방향을 **바라보고 이동**하여 공격합니다.  
  ![targeting1](https://github.com/user-attachments/assets/ba99296e-fae6-42c0-a498-3bdc6b6a7964) ![targeting2](https://github.com/user-attachments/assets/6a8e3627-a140-4a27-8c28-34492e6399a6)

- 적 캐릭터는 AI 기능을 활용하여 구현하였습니다.
- 3명의 플레이어블 캐릭터에게 고유한 일반공격, 스킬, 궁극기를 구현하였습니다.
![image](https://github.com/user-attachments/assets/0a347fc7-97df-4b4b-9076-fbded3c64416)


### 🔹 대쉬, 점프, 스프린트
- 모든 플레이어 캐릭터가 `Dash()`, `Jump()`, `Sprint()` 공통 기능을 보유  
- 상속 구조로 구성하여 **블루프린트에서 모듈화 가능**  

### 🔹 공격 시스템
| 공격 유형 | 구현 방식 | 관련 기술 |
|---------|--------|---------|
| **일반 공격** | `AimingComponent`에서 적 타겟팅 후 방향과 위치 조정 | `ActionComponent` |
| **콤보 시스템** | `bCanCombo` 변수를 활용하여 연속 공격 가능 | `Notify` 활용 |
| **스킬 시스템** | `CCooldownManager`를 활용하여 쿨타임 관리 | `UObject` 상속 |
| **궁극기 시스템** | 전용 카메라 액션 적용 (`SetViewTarget`) | `AnimNotify` |


---

## 4. 상호작용 시스템 (Interaction System)
### 🛠 동작 방식
- `UCInteractionInterface` 인터페이스를 상속받아 `Interact()` 함수 구현  
- `F` 키 입력 시 `InteractionComponent`에서 Trace하여 주변의 상호작용 가능한 액터 탐색  
- 서버 RPC 함수로 실행하여 모든 클라이언트에서 일관된 상호작용 보장 [RPC 패턴 사용](#0-멀티플레이-동기화-Multiplayer-Game-Sync-Techniques)

![image](https://github.com/user-attachments/assets/14162f50-8d52-4908-8889-e5474d82bedc)

| 상호작용 대상 | 동작 |
|-------------|-----|
| **보물상자 (Chest)** | 상호작용 시 보물상자가 열리며 아이템 스폰 |
| **아이템 (ItemBase)** | 플레이어 인벤토리에 아이템 추가 |

- 인벤토리의 아이템 갯수는 변수 Replication 사용 [Replication 패턴 사용](#0-멀티플레이-동기화-Multiplayer-Game-Sync-Techniques)

---

## 5. 적 AI 시스템 (Enemy AI System)
### 🛠 언리얼 AI 및 리스폰 기능
- `EnemyCharacter` 클래스로 구성, `NPCActionComponent`에서 공격 실행  
- `AIController` 및 `Behavior Tree`를 활용하여 AI 상태 관리  
- `EnemySpawner`를 활용하여 적 스폰 및 자동 리스폰 구현
![image](https://github.com/user-attachments/assets/0397e47c-5e99-4771-b5cd-1d231b2d8587)


---
## 6. 미니맵 및 UI 시스템
### 🛠 실시간 미니맵과 인벤토리의 동적 로딩
- `SceneCaptureComponent2D`를 활용한 실시간 미니맵 구현  
- `WB_MainHUD`에서 스킬, 체력, 캐릭터 교체 UI 관리  
- `Soft References`를 활용하여 UI 최적화 및 데이터 로딩 감소  
![image](https://github.com/user-attachments/assets/69de4475-3bf5-4178-944b-883068b2d23a)

- Inventory UI 에서 포션과 같은 소비아이템 사용가능 (아이템의 효과 생성)
---

## 7. 데이터 및 에셋 관리
### 🛠 데이터 테이블을 통한 관리
- 캐릭터의 GameAblitySystem의 **AttributeSet을 위한 Attribute(HP 등의 수치)** 데이터 테이블  
  ![Attribute](https://github.com/user-attachments/assets/4d75a88c-99b2-4d3c-94f1-b058029408e3)  

- **아이템의 각 정보**를 담은 ItemInfo 및 ConsumableItemInfo(소비) 데이터 테이블  
  ![Item](https://github.com/user-attachments/assets/843c93e9-040e-4686-8443-7454e871586b)  


### 🛠 데이터 에셋을 통한 관리
- 캐릭터의 각 **액션(전투 스킬 등)**마다 사용하는 사거리 등 **수치 데이터** 및 **몽타주, 이펙트 등의 에셋**을 관리하는 데이터 에셋
  ![ActionDataAsset](https://github.com/user-attachments/assets/f847bab2-a9ad-4132-bdb8-c04bb7f83c0f)




## 8. 로그인 및 게임 참여 시스템
### 🛠 DB 시스템을 활용한 로그인 인증
- `Flask` 서버와 `MySQL`을 활용하여 HTTP `POST` 방식으로 유저 데이터 인증  
- `GameInstance`에 `DBManager`를 생성하여 유저 데이터 관리
  
#### 📌 클라이언트-서버-데이터베이스 요청 흐름
| 클라이언트       | 서버 (Backend)                  | 데이터베이스 (DB)            |
|------------------|----------------------------------|------------------------------|
| POST 요청 전송 🡆 |                                 |                              |
|                  |    요청을 처리,DB에 쿼리 실행 🡆  |                              |
|                  |                                  | 🡆 쿼리 실행 및 결과 반환    |
|                  |     🡄 DB 응답을 받아 처리        |                             |
| 🡄 서버 응답 반환 |                                  |                             |

![image](https://github.com/user-attachments/assets/2d3967e3-ffdb-403a-bb52-e0e04ca22a40)

---

## 9. 포트폴리오 영상 & 코드 링크
- 📌 **[포트폴리오 영상 보기](https://youtu.be/xxxxxxx](https://youtu.be/nGRqgbBO1Gg))**  
- 📌 **[GitHub 소스코드 확인](https://github.com/HyeonLang/UE4.25_RPG/tree/main/Source)**  
- 📌 **[게임 데모 다운로드](https://github.com/HyeonLang/UE4.25_RPG)**  
