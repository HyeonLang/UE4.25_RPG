# UE4.25_RPG

 5주차 구현

playercontroller : Jump 구현, Dash는 애니메이션만 입력하면 됨.

character : 3가지 캐릭터들을 모두 적용. 캐릭터 변환 및 이동이 버그없이 자연스럽게 동작함
            카메라가 메쉬에 막혀 가까히 보이는 현상을 수정. 새로운 콜리전 프로파일을 만들어 캐릭터에 적용
skill: 캐릭터들이 각각 어떤 공격과 스킬을 사용할지와 사용에 따른 애니메이션을 결정
component :  skill component 및 action 컴포넌트 생성 둘을 통합할까도 생각중

다음주차 구현

component :  skill component 및 action 컴포넌트 를 정리하고 스킬을 구현, 

character : 캐릭터의 스킬에 대한 설계 -> 스킬에 필요한 데이터를 담는 데이터 테이블 설계 세가지 캐릭터들의 스킬 몽타주 제작 및 구현

gamemode : 멀티플레이를 위한 리슨서버 구현 및 rpc, replicate 설정
