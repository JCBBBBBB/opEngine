#pragma once
#include "../opEngine_SOURCE/opScene.h"

namespace op
{
	class ResultScene : public Scene
	{
	public:
		ResultScene();
		~ResultScene();

		void LateUpdate() override;
		void Render(HDC hdc) override;

		// 플레이 씬에서 최종 결과를 전달받는다
		void SetResult(int score, int stage, bool isClear);
	
	private:
		int m_score; // 최종 점수
		int m_stage; // 종료한 스테이지
		bool m_isClear; // 전체 클리어인지
	};
}
