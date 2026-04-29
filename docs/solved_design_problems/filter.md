
문제: is등이 가장 까가운 array를 필터링한다면 array를 여러개쓰면 처리할 수 없다.
가장 가까운 array대신 처음 array를 필터링하자!
그럼 특정 플레이어의 아이템 중 어떤 아이템에 대해 효과를 적용할 수 없다 -> 이건 원래도 안 됐다

entities.name.is "playername" | inventory.items.type.is dirt | [여기서 player인가 dirt인가?]
해석1: 이름이 ""인 entity 중 dirt를 가진 player.
해석2: 이름이 ""인 entity의 dirt 아이템

결정: 해석 2를 남긴다.
이유: 해석 1은 다른 방법으로 구현이 가능하다
entities.name.is "playername" | inventory.has dirt |

즉 가장 나중에 온 Array를 기준으로 필터링해 남긴다.

해석 1을 남긴다면 2를 구현하기 위해 "모든 엔티티에 적용되는 함수" ex: filter등 을 추가해야 하기에 선택지 복잡도를 늘린다.
필터링 기능은 가장 말단의 is/has/not 등등의 함수에게 이관한다.

추가 결정: Filter는 별도 derived type으로 두지 않는다.
필터링은 `Array<T>` 위에서 동작하는 조건 함수의 의미로 처리하고, 타입 시스템에는 `Array<T>`만 derived type으로 남긴다.
