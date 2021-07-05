/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */
#pragma once
#include "Containers/ContainersFwd.h"

//Temporary library for std container support
#include <type_traits>

namespace cave
{
	template<typename Iterator>
	class TIsIterator
	{
		static constexpr bool Value = std::_Is_iterator_v<Iterator>;
	};

	template<typename ContainerType, typename ElementType, typename SizeType>
	class TIsIterator<TArrayIterator<ContainerType, ElementType, SizeType>>
	{
		static constexpr bool Value = true;
	};
}
