# UE4.25_RPG

 4주차 구현

 playercontroller : playercontroller 단에 필요한 스탯을 가지는 controllerstate 컴포넌트 제작

 character : 필요한 스탯이 있는 attribute컴포넌트와 상태가 있는 state 컴포넌트 제작
            캐릭터 파라곤의 애니메이션 인스턴스가 모두 같은것을 이용하여 CAiminstance 제작
            캐릭터가 타겟팅할 적과 방향을 정하는 AimingComponent 제작, 몬스터에도 사용할 수 있게 제작

 다음주차 구현 

 character : 캐릭터의 스킬에 대한 설계 -> 스킬에 필요한 데이터를 담는 데이터 테이블 설계
             세가지 캐릭터들의 스킬 몽타주 제작
 
 gamemode : 멀티플레이를 위한 리슨서버 구현 및 rpc, replicate 설정
