/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */
#include <iterator>

export module IteratorType;

namespace cave
{
	export template<class Iter>
	concept IterType = TIsIterator<Iter>::Value;

	template<class Iter>
	concept StdIterType = std::_Is_iterator_v<Iter>;

	export template<class Iter>
	struct TIsIterator
	{
		static constexpr bool Value = false;
	};

	export template<StdIterType Iter>
	struct TIsIterator<Iter>
	{
		static constexpr bool Value = true;
	};

	
}