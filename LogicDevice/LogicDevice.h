#pragma once

/*******************************
Logic Device
�� ����
�ݵ�ü ���ڸ� ��Ƽ��� ������ �� ����ü Ȥ�� Ŭ����
���� ������ ���� ź�������� ����Ȯ������ ���� ���� �����Ͽ� ����� �� �ֽ��ϴ�.

���� ���̵�
1. ���ӽ����̽��� ������ �̸��� LD(LogicDevice)��� ���ӽ����̽��� ���� �ʵ忡�� ����ϴ�.
2. ����ü Ȥ�� Ŭ������ �����մϴ�. ĸ��ȭ�� ���� Ŭ������ ��õ�մϴ�.
3. ���� ����̽��� ������ �µ��� �ʿ� ��� ���� ���� �߰����� �ʵ��� �մϴ�. �ʿ��ϸ� ���ο� ���� ����̽��� ���� ���� ��õ�մϴ�.
4. �ʿ信 ���� ���ø����� �����մϴ�.
5. �̱� ����� �ۼ��ϵ��� �մϴ�.

���� ���۵� ����
1. Gauge (������)
2. RoundTripGauge (�պ� ������)
3. TimeChecker (�ð� üĿ)
4. TimeAccumulator (�ð� ������)

*******************************/

namespace LHJ { namespace LD {

	/// <summary>
	/// Gauge�� HP, MP�� ���� 0���� Max�������� ���� ������ ��ü�Դϴ�.
	/// �ð���ȭ���� ����ϱ� ������ �� ������ ������Ʈ�� �ʿ��մϴ�.
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
		float Cur = 0.f;	// ���簪
		float Max = 0.f;	// �ִ밪
		float Prev = 0.f;	// �ش� ���ް��� ���� ���ʸ� üũ�ϱ� ���� ����մϴ�.

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
		// �� ��� Update
		bool Increase(const float& fTimeDelta, bool OnlyCheckMax = true)
		{
			Prev = Cur;
			Cur += fTimeDelta;

			if (Cur >= Max)
			{
				Cur = Max;
				return true;
			}
			// 0���ϰ� ����
			else if (Cur <= 0.f)
			{
				Cur = 0.f;

				if (!OnlyCheckMax)
					return true;
			}

			return false;
		}

		// �� �϶� Update
		bool Decrease(const float& fTimeDelta, bool OnlyCheckZero = true)
		{
			Prev = Cur;
			Cur -= fTimeDelta;

			if (Cur <= 0.f)
			{
				Cur = 0.f;
				return true;
			}
			// Max �̻� ����
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

		// Max�϶�
		bool IsMax()
		{
			return (Cur >= Max);
		}

		// Max ���޽� �ѹ��� üũ
		bool IsMaxOnce()
		{
			return Once() && (Cur >= Max);
		}

		// Ư�������� Ŭ��
		bool IsUpper(const float value)
		{
			return (Cur >= value);
		}

		bool IsUpperOnce(const float value)
		{
			return Once() && (Cur >= value);
		}

		// 0�� ��
		bool IsZero()
		{
			return (Cur == 0.f);
		}

		// 0�� ���޽� �ѹ��� üũ
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

		// ���� ���ϴ� �Լ�
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

	// �뵵�� �ٸ��� �����ü�� ������ ��쿡 �ٸ� �̸����� ����� �� �ֽ��ϴ�.
	using Delay = Gauge;


	/// <summary>
	/// �ð��� �������� üŷ�ϴ� ����
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

// using namespace LHJ -> ������ �ĺ��� ���� ����, �����ڿ� ���� �ٸ� ���ӽ����̽��� ����մϴ�.
// using namespace LD -> �� ���ӽ����̽��� using�� ������� �ʰ� ������ ��õ