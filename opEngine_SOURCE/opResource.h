#pragma once
#include "opEntity.h"

namespace op
{
	// 개별 리소스의 부모
	class Resource abstract : public Entity
	{
	public:
		Resource(enums::ResourceType resourceType);
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0; // 순수 가상함수

		const std::wstring& GetPath() { return m_path; }
		void SetPath(const std::wstring& path) { m_path = path; }

	private:
		const enums::ResourceType m_type;
		std::wstring m_path;
	};
}

// 게임에서 사용되는 데이터들의 종류
// 이미지, 사운드, 3스테이지