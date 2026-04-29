
world.blocks.pos.x 50 | y 100 | z 0 | setblock world.blocks.pos.x 13 | y 20 | z 50 | pos.to.x 17 | y 30 | z 65 | mask.not air
world.blocks.pos.x 50 | y 100 | z 0 | setblock sand

[x,y,z] ... [x,y,z] 형태로 튜플이 들어가야 한다면 1-hand 인자 구조가 병목이 된다.
하지만 이를 위해 다인자 함수를 만들면 |과 .만으로 함수 호출을 구성할 수 없게 되고 (ambiguous grammar)
    id2 id1 id0 | id1 id0 | id0 | 에서
    id2   [id1 id0 |]   [id1 id0 | id0]   | 과
    id2   [id1 id0 | id1 id0 |]   [id0]   | 이 중복
    함수 호출에 |.를 써서 막아야 하는데 이는 어색하다.

    entities.name.is "name" |.damage 10

    setblock(world.blocks.pos(13,20,50).pos.to(17,30,65).mask.not(air))
    => setblock world.blocks.pos 13 20 50 |.pos.to 17 30 65 |.mask.not air |

blank name function을 도입해서
world.blocks.pos 의 리턴타입: XInputProxy<Array<Block>>
해당 타입은 유일한 함수를 가짐: 이름없는 함수
해당함수는 1-arg, 리턴타입은 YInputProxy<Array<Block>>
즉 이렇게 하면 다음과 같이 가능

world.blocks.pos 50 | 100 | 50 | setblock world.blocks.pos 3 | 20 | 50 | pos.to 17 | 30 | 65 | mask.not air

단점은 BNF로 문법을 나타내지 못함. semantics가 같이 들어간 방식으로 ast를 만들어야 함.
하지만 설계 난이도나 에러 메시지 가시성상 전통적 파서 인프라 대신 직접 파서를 짤 예정이므로 문제의 여지 적을 수 있음. 요소 개수가 적고 함수가 1-arg라 스택 하나로 binary tree만 추적하면 됨.