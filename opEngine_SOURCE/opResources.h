#pragma once
#include "opResource.h"

namespace op
{
	// 여러 리소스를 관리하는 클래스
	class Resources
	{
	public:
		template <typename T>
		static T* Find(const std::wstring& key)
		{
			auto iter = m_resources.find(key); // key와 같은 이름의 항목 찾기
			if (iter == m_resources.end())
			{
				return nullptr; // 찾지 못함
			}

			return dynamic_cast<T*>(iter->second); // 찾은 리소스 주소를 원하는 타입으로 변환
		}

		template <typename T>
		static T* Load(const std::wstring& key, const std::wstring& path)
		{
			T* resource = Resources::Find<T>(key); // 같은 이름과 요청한 타입의 리소스가 이미 있는지 확인
			if (resource != nullptr)
			{
				return resource; // 있으면 기존 것을 재사용
			}

			resource = new T(); // 없으면 새 리소스 생성
			if (FAILED(resource->Load(path)))
			{
				assert(false);
			}

			resource->SetName(key); // 리소스의 이름 설정
			resource->SetPath(path); // 파일 경로 기록
			m_resources.insert(std::make_pair(key, resource)); // 이름과 주소를 한 쌍으로 만들어 목록에 등록

			return resource;
		}

		static void Release()
		{
			for (auto& iter : m_resources)
			{
				delete iter.second;
				iter.second = nullptr;
			}
		}

	private:
		static std::map<std::wstring, Resource*> m_resources;
	};
}