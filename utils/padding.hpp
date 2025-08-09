#pragma once
#include <type_traits>

#define COMBINE( x, y ) x##y
#define COMBINE2( x, y ) COMBINE( x, y )

#define PAD( sz ) \
	private: \
	std::uint8_t COMBINE2( pad_, __COUNTER__ )[ sz ]; \
	public:

#define ENUM_BITMASK(e)															\
																				\
inline constexpr e operator |( e lhs, e rhs ) {					\
	return static_cast< e > (											\
		static_cast< std::underlying_type< e >::type >( lhs ) |			\
		static_cast< std::underlying_type< e >::type >( rhs )				\
		);																		\
}																				\
																				\
inline constexpr e operator &( e lhs, e rhs ) {					\
	return static_cast< e > (											\
		static_cast< std::underlying_type< e >::type >( lhs ) &			\
		static_cast< std::underlying_type< e >::type >( rhs )				\
		);																		\
}																				\
																				\
inline constexpr e operator ^( e lhs, e rhs ) {					\
	return static_cast< e > (											\
		static_cast< std::underlying_type< e >::type >( lhs ) ^			\
		static_cast< std::underlying_type< e >::type >( rhs )				\
		);																		\
}																				\
																				\
inline constexpr e operator ~( e rhs ) {									\
	return static_cast< e > (											\
		~static_cast< std::underlying_type< e >::type >( rhs )			\
		);																		\
}																				\
																				\
inline constexpr e& operator |=( e& lhs, e rhs ) {				\
	lhs = static_cast< e > (											\
		static_cast< std::underlying_type< e >::type >( lhs ) |			\
		static_cast< std::underlying_type< e >::type >( rhs )				\
		);																		\
																				\
	return lhs;																	\
}																				\
																				\
inline constexpr e& operator &=( e& lhs, e rhs ) {				\
	lhs = static_cast< e > (											\
		static_cast< std::underlying_type< e >::type >( lhs ) &			\
		static_cast< std::underlying_type< e >::type >( rhs )				\
		);																		\
																				\
	return lhs;																	\
}																				\
																				\
inline constexpr e& operator ^=( e& lhs, e rhs ) {				\
	lhs = static_cast< e > (											\
		static_cast< std::underlying_type< e >::type >( lhs ) ^				\
		static_cast< std::underlying_type< e >::type >( rhs )				\
		);																		\
																				\
	return lhs;																	\
}			\
inline bool operator!(e x) {\
return x == static_cast< e >( 0 );\
}