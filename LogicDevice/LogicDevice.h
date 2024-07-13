#pragma once

/*******************************
Logic Device
논리 소자
반도체 소자를 모티브로 제작한 논리 구조체 혹은 클래스
게임 제작을 위해 탄생했지만 개념확장으로 여러 논리에 보조하여 사용할 수 있습니다.

제작 가이드
1. 네임스페이스에 제작자 이름과 LD(LogicDevice)라는 네임스페이스로 감싼 필드에서 만듭니다.
2. 구조체 혹은 클래스로 제작합니다. 캡슐화를 위해 클래스를 추천합니다.
3. 로직 디바이스는 목적에 맞도록 필요 기능 외의 것을 추가하지 않도록 합니다. 필요하면 새로운 로직 디바이스를 만들 것을 추천합니다.
4. 필요에 따라 템플릿으로 제작합니다.
5. 싱글 헤더로 작성하도록 합니다.

사전 제작된 소자
1. Gauge (게이지)
2. RoundTripGauge (왕복 게이지)
3. TimeChecker (시간 체커)
4. TimeAccumulator (시간 누적기)

*******************************/

namespace LHJ { namespace LD {

	/// <summary>
	/// Gauge는 HP, MP와 같은 0부터 Max값까지의 값을 가지는 객체입니다.
	/// 시간변화율을 사용하기 때문에 매 프레임 업데이트가 필요합니다.
	/// </summary>
	class Gauge
	{
	public:
		Gauge() {}
		Gauge(float value)
			: Max(value) {}
		Gauge(const Gauge& rhs)
			: Cur(rhs.Cur), Max(rhs.Max), Prev(rhs.Prev)
		{}
		~Gauge() {}

	private:
		float Cur = 0.f;	// 현재값
		float Max = 0.f;	// 최대값
		float Prev = 0.f;	// 해당 도달값에 대한 최초를 체크하기 위해 사용합니다.

	public:
		Gauge operator=(const Gauge& value)
		{
			return Gauge(value);
		}
		Gauge operator=(const float value)
		{
			Max = value;

			return (*this);
		}

	public:
		// 값 상승 Update
		bool Increase(const float& fTimeDelta, bool OnlyCheckMax = true)
		{
			Prev = Cur;
			Cur += fTimeDelta;

			if (Cur >= Max)
			{
				Cur = Max;
				return true;
			}
			// 0이하값 방지
			else if (Cur <= 0.f)
			{
				Cur = 0.f;

				if (!OnlyCheckMax)
					return true;
			}

			return false;
		}

		// 값 하락 Update
		bool Decrease(const float& fTimeDelta, bool OnlyCheckZero = true)
		{
			Prev = Cur;
			Cur -= fTimeDelta;

			if (Cur <= 0.f)
			{
				Cur = 0.f;
				return true;
			}
			// Max 이상값 방지
			else if (Cur >= Max)
			{
				Cur = Max;

				if (!OnlyCheckZero)
					return true;
			}

			return false;
		}

		void Reset()
		{
			Cur = 0.f;
			Prev = 0.f;
		}

		void ResetToMax()
		{
			Cur = Max;
			Prev = Max;
		}

		void SetCur(const float value)
		{
			Prev = Cur;
			Cur = value;
		}

		// Max일때
		bool IsMax()
		{
			return (Cur >= Max);
		}

		// Max 도달시 한번만 체크
		bool IsMaxOnce()
		{
			return Once() && (Cur >= Max);
		}

		// 특정값보다 클때
		bool IsUpper(const float value)
		{
			return (Cur >= value);
		}

		bool IsUpperOnce(const float value)
		{
			return Once() && (Cur >= value);
		}

		// 0일 때
		bool IsZero()
		{
			return (Cur == 0.f);
		}

		// 0에 도달시 한번만 체크
		bool IsZeroOnce()
		{
			return Once() && (Cur <= 0.f);
		}

		bool IsLower(const float value)
		{
			return (Cur <= value);
		}

		bool IsLowerOnce(const float value)
		{
			return Once() && (Cur <= value);
		}

		// 비율 구하는 함수
		float Percent()
		{
			return (Max != 0.f) ? (Cur / Max) : (0.f);
		}

	private:
		bool Once()
		{
			return (Prev != Cur);
		}
	};

	// 용도가 다르나 기능자체는 동일할 경우에 다른 이름으로 사용할 수 있습니다.
	using Delay = Gauge;


	/// <summary>
	/// 시간이 지났는지 체킹하는 소자
	/// </summary>
	class TimeChecker
	{
	public:
		TimeChecker() {}
		TimeChecker(float fInterval)
			: fMax(fInterval) {}
		TimeChecker(const TimeChecker& rhs)
			: fTime(rhs.fTime), fMax(rhs.fMax) {}
		~TimeChecker() {}

	private:
		float	fTime = { 0.f };
		float	fMax = { 0.f };
		bool	bTicked = { false };

	public:
		TimeChecker operator =(float fValue)
		{
			fMax = fValue;
			fTime = 0.f;
			bTicked = false;

			return (*this);
		}

	public:
		bool Update(const float& fTimeDelta)
		{
			if (bTicked)
				bTicked = false;

			bool bIsChecked = false;
			fTime += fTimeDelta;
			while (fTime >= fMax)
			{
				fTime -= fMax;
				bIsChecked = true;
			}

			return bTicked = bIsChecked;
		}

		bool IsTicked()
		{
			return bTicked;
		}

	};
}}

// using namespace LHJ -> 제작자 식별을 위해 쓰임, 제작자에 따라 다른 네임스페이스를 사용합니다.
// using namespace LD -> 이 네임스페이스는 using을 사용하지 않고 쓸것을 추천