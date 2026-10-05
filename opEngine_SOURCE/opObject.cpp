#include "opObject.h"

namespace op::object
{
    // 원본의 향후 씬 유지 오브젝트 목록. 이 커밋에서는 아직 사용하지 않는다.
    static std::vector<GameObject*> DontDestroyGameObjects = {};
}
