#ifndef TRANTOR_NON_COPYABLE_H_
#define TRANTOR_NON_COPYABLE_H_

namespace trantor
{
	class TrantorNonCopyable
	{
	public:
		TrantorNonCopyable() = default;
		~TrantorNonCopyable() = default;
	private:
		TrantorNonCopyable(const TrantorNonCopyable&) = delete;
		TrantorNonCopyable& operator=(const TrantorNonCopyable&) = delete;
	};
}

#endif
