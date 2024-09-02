# UE4.25_ActionGame

 2주차 구현
gamemodebase : playercharacter들을 스폰하고 controller에 possess

playercontroller: playercharacter 배열에 character를 받아 그것들을 동작하게 하는 입력을 받음.
                  playercharacter들의 빙의를 변경시켜 캐릭터 교체 기능을 만듬.

character : 캐릭터들의 기본 틀을 구현

다음 주차 예정

playercontroller : 한개의 캐릭터가 이동을 하는동안 다른 캐릭터들을 같은 위치로 이동하도록 수정
                   playercontroller 단에 필요한 스탯을 가지는 controllerstate 컴포넌트 제작

character : 필요한 스탯이 있는 attribute컴포넌트와 상태가 있는 state 컴포넌트 제작
            에셋을 모두 구하기 전에 테스트할 마네킹의 이동 애니메이션 인스턴스 제작
