#pragma once

#if !defined(_STL_EXTRA_DISABLED_WARNINGS)
#	define _STL_EXTRA_DISABLED_WARNINGS 4061 4324 4365 4514 4571 4582 4583 4623 4625 4626 4710 4774 4820 4987 5026 5027 5039
#endif

#if !defined(_SCL_SECURE_NO_WARNINGS)
#	define _SCL_SECURE_NO_WARNINGS 1
#endif

#if !defined(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
#	define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING 1
#endif

#include <algorithm>
#include <array>
#include <functional>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>

#pragma warning(disable : 4324) // warning C4234: structure was padded due to alignment specifier
#pragma warning(disable : 4514) // warning C4514: '%s': unreferenced inline function has been removed
#pragma warning(disable : 4625) // warning C4625: '%s': copy constructor was implicitly defined as deleted
#pragma warning(disable : 4626) // warning C4626: '%s': assignment operator was implicitly defined as deleted
#pragma warning(disable : 4710) // warning C4710: '%s': function not inlined
#pragma warning(disable : 4820) // warning C4820: '%s': '%d' bytes padding added after data member '%s'
#pragma warning(disable : 5026) // warning C5026: '%s': move constructor was implicitly defined as deleted
#pragma warning(disable : 5027) // warning C5027: '%s': move assignment operator was implicitly defined as deleted

#pragma warning(disable : 26412) // warning C26412: Do not dereference an invalid pointer (lifetimes rule 1). 'return of %s' was invalidated at line %d by 'no initialization'.
#pragma warning(disable : 26481) // warning C26481: Don't use pointer arithmetic. Use span instead. (bounds.1: http://go.microsoft.com/fwlink/p/?LinkID=620413)
#pragma warning(disable : 26485) // warning C26485: Expression '%s::`vbtable'': No array to pointer decay. (bounds.3: http://go.microsoft.com/fwlink/p/?LinkID=620415)
#pragma warning(disable : 26490) // warning C26490: Don't use reinterpret_cast. (type.1: http://go.microsoft.com/fwlink/p/?LinkID=620417)
#pragma warning(disable : 26499) // warning C26499: Could not find any lifetime tracking information for '%s'

#include <gsl/gsl>

#pragma pointers_to_members(full_generality, virtual_inheritance)

namespace md
{
	template<typename T, ptrdiff_t E = gsl::dynamic_extent>
	using span = gsl::span<T, E>;

	constexpr ptrdiff_t dynamic_extent = gsl::dynamic_extent;

	namespace
	{
#pragma warning(push)
#pragma warning(disable : 6326) // warning C6326: Potential comparison of a constant with another constant.
		template<ptrdiff_t... Ns>
		constexpr size_t count_dynamics() {
			size_t count  = 0;
			using swallow = size_t[];
			static_cast<void>(swallow{ (count += (Ns == dynamic_extent))... });
			return count;
		}
#pragma warning(pop)

		template<template<typename...> typename Pred, typename... Types>
		constexpr std::size_t find_first_type_of() {
			constexpr bool matches[] = { Pred<Types>::value... };
			for(std::size_t i = 0; i < sizeof...(Types); ++i) {
				if(matches[i]) {
					return i;
				}
			}
			return ~static_cast<std::size_t>(0);
		}

		template<template<typename...> typename Pred, typename... Types>
		constexpr std::size_t find_first_type_of_v = find_first_type_of<Pred, Types...>();

		template<std::size_t I, typename... Types>
		struct select_nth_type;

		template<typename T, typename... Ts>
		struct select_nth_type<0, T, Ts...>
		{
			using type = T;
		};

		template<std::size_t I, typename T, typename... Ts>
		struct select_nth_type<I, T, Ts...> : select_nth_type<I - 1, Ts...>
		{
			static_assert(I <= sizeof...(Ts), "bad index");
		};

		template<size_t I, typename... Ts>
		using select_nth_t = typename select_nth_type<I, Ts...>::type;

		template<template<typename...> typename Predicate, typename... Ts>
		struct first_matching_type
		{
			using type = select_nth_t<find_first_type_of_v<Predicate, Ts...>, Ts...>;
		};

		template<template<typename...> typename Predicate, typename... Ts>
		using first_matching_type_t = typename first_matching_type<Predicate, Ts...>::type;

		template<size_t I, typename T, typename... Ts>
		constexpr auto select_nth_value_aux(T&& value, Ts&&... values) {
			if constexpr(I == 0) {
				return std::forward<T>(value);
			} else {
				return select_nth_value_aux<I - 1>(std::forward<Ts>(values)...);
			}
		}

		template<size_t I, typename... Ts>
		constexpr auto select_nth_value(Ts&&... values) {
			return select_nth_value_aux<I>(std::forward<Ts>(values)...);
		}
	}

	template<ptrdiff_t... Extents>
	struct extents
	{
		using index_type = ptrdiff_t;
		using array_type = index_type[];
		using my_type    = extents<Extents...>;

		constexpr extents() noexcept : values{ Extents... } {
		}
		constexpr extents(const extents&) = default;
		constexpr extents(extents&&)      = default;
		~extents()                        = default;
		extents& operator=(const extents&) noexcept = default;
		extents& operator=(extents&&) noexcept = default;

		static constexpr size_t rank() noexcept {
			return sizeof...(Extents);
		}

		static constexpr size_t rank_dynamic() noexcept {
			return count_dynamics<Extents...>();
		}

		static constexpr index_type static_extent(size_t e) noexcept {
			if(e >= rank()) {
				return 1;
			} else {
				return array_type{ Extents... }[e];
			}
		}

		constexpr index_type extent(size_t e) const noexcept {
			if(e >= rank()) {
				return 1;
			} else {
				return values[e];
			}
		}

		constexpr size_t size() const noexcept {
			return static_cast<size_t>(accumulate(values.cbegin(), values.cend(), static_cast<index_type>(1), std::multiplies<>()));
		}

		template<typename IndexType, size_t N>
		constexpr extents(const std::array<IndexType, N>& dynamic_extents) noexcept : values{ Extents... } {
			static_assert(N == rank_dynamic(), "wrong number of dynamic extents provided");
			for(size_t i = 0, j = 0; i < rank(); ++i) {
				if(values[i] == dynamic_extent) {
					values[i] = dynamic_extents[j++];
				}
			}
		}
		template<typename... IndexType>
		constexpr extents(IndexType... DynamicExtents) noexcept :
		extents(std::array<index_type, sizeof...(IndexType)>{ DynamicExtents... }) {
		}

	private:
		std::array<index_type, sizeof...(Extents)> values;
	};

	template<typename E, typename = void>
	struct is_extent : std::false_type
	{
	};

	template<typename E>
	struct is_extent<E,
	                 std::void_t<typename E::index_type,
	                             decltype(E::rank()),
	                             decltype(E::rank_dynamic()),
	                             decltype(E::static_extent(std::declval<size_t>())),
	                             decltype(std::declval<E>().extent(std::declval<size_t>())),
	                             decltype(std::declval<E>().size())>> : std::true_type
	{
	};

	template<typename E>
	constexpr bool is_extent_v = is_extent<E>::value;

	namespace
	{
		template<typename Dimensions>
		struct mapping_base
		{
			using index_type = ptrdiff_t;
			using array_type = index_type[];

			using extent_type = Dimensions;

			constexpr mapping_base() noexcept                    = default;
			constexpr mapping_base(const mapping_base&) noexcept = default;
			constexpr mapping_base(mapping_base&&) noexcept      = default;
			~mapping_base() noexcept                             = default;
			mapping_base& operator=(const mapping_base&) noexcept = default;
			mapping_base& operator=(mapping_base&&) noexcept = default;

			template<typename IndexType, size_t N>
			constexpr mapping_base(const std::array<IndexType, N>& dynamic_extents) noexcept : dimensions(dynamic_extents), strides{} {
			}

			template<typename... IndexType>
			constexpr mapping_base(IndexType... DynamicExtents) noexcept :
			mapping_base(std::array<index_type, sizeof...(DynamicExtents)>{ DynamicExtents... }) {
			}

			static constexpr size_t rank() noexcept {
				return extent_type::rank();
			}
			static constexpr size_t rank_dynamic() noexcept {
				return extent_type::rank_dynamic();
			}

			static constexpr index_type static_extent(size_t i) noexcept {
				return extent_type::static_extent(i);
			}

			constexpr index_type extent(size_t i) const noexcept {
				return dimensions.extent(i);
			}

			constexpr size_t span_size() const noexcept {
				return dimensions.size();
			}

			static constexpr bool is_always_unique     = true;
			static constexpr bool is_always_contiguous = true;
			static constexpr bool is_always_strided    = true;

			constexpr bool is_unique() const noexcept {
				return true;
			}

			constexpr bool is_contiguous() const noexcept {
				return true;
			}

			constexpr bool is_strided() noexcept {
				return true;
			}

			constexpr index_type stride(size_t r) const noexcept {
				if(r >= rank()) {
					return 0;
				}
				return strides[r];
			}

			template<typename IndexType, size_t N>
			constexpr index_type operator()(const std::array<IndexType, N>& idxes) const noexcept {
				static_assert(N == rank(), "wrong number of indices passed");
				return inner_product(strides.begin(), strides.end(), idxes.begin(), gsl::narrow<index_type>(0), std::plus<index_type>(), std::multiplies<index_type>());
			}

			template<typename... IndexType>
			constexpr index_type operator()(IndexType... indices) const noexcept {
				return operator()(std::array<index_type, sizeof...(IndexType)>{ gsl::narrow<index_type>(indices)... });
			}

		protected:
			extent_type dimensions;
			std::array<size_t, extent_type::rank()> strides;
		};
	}

	struct layout_right
	{
		template<typename Dimensions>
		struct mapping : mapping_base<Dimensions>
		{
			using base_type = mapping_base<Dimensions>;

			using index_type = typename base_type::index_type;

			constexpr mapping() noexcept : mapping(std::array<index_type, 0>{}) {
			}
			constexpr mapping(const mapping&) noexcept = default;
			constexpr mapping(mapping&&) noexcept      = default;
			~mapping() noexcept                        = default;
			mapping& operator=(const mapping&) noexcept = default;
			mapping& operator=(mapping&&) noexcept = default;

			template<typename IndexType, size_t N>
			constexpr mapping(const std::array<IndexType, N>& dynamic_extents) noexcept : base_type(dynamic_extents) {
				size_t stride = 1;
				for(size_t i = 0; i < base_type::rank(); ++i) {
					size_t j         = base_type::rank() - 1 - i;
					this->strides[j] = stride;
					stride *= this->dimensions.extent(j);
				}
			}

			template<typename... IndexType>
			constexpr mapping(IndexType... DynamicExtents) noexcept : mapping(std::array<index_type, sizeof...(DynamicExtents)>{ DynamicExtents... }) {
			}
		};
	};

	struct layout_left
	{
		template<typename Dimensions>
		struct mapping
		{
			using base_type = mapping_base<Dimensions>;

			using index_type = typename base_type::index_type;

			constexpr mapping() noexcept : mapping(std::array<index_type, 0>{}) {
			}
			constexpr mapping(const mapping&) noexcept = default;
			constexpr mapping(mapping&&) noexcept      = default;
			~mapping() noexcept                        = default;
			mapping& operator=(const mapping&) noexcept = default;
			mapping& operator=(mapping&&) noexcept = default;

			template<typename IndexType, size_t N>
			constexpr mapping(const std::array<IndexType, N>& dynamic_extents) noexcept : base_type(dynamic_extents) {
				size_t stride = 1;
				for(size_t i = 0; i < base_type::rank(); ++i) {
					this->strides[i] = stride;
					stride *= this->dimensions.extent(i);
				}
			}

			template<typename... IndexType>
			constexpr mapping(IndexType... DynamicExtents) noexcept : mapping(std::array<index_type, sizeof...(DynamicExtents)>{ DynamicExtents... }) {
			}
		};
	};

	template<size_t... Strides>
	struct layout_strided
	{
		template<typename Dimensions>
		struct mapping
		{
			// TODO this is rather ill-defined. For padded mdarrays the stride should be *bytes*, not *elements*
			// but the other layouts operate in *elements* not *bytes*.
		};
	};

	template<typename M, typename = void>
	struct is_mapping : std::false_type
	{
	};

	template<typename M>
	struct is_mapping<M,
	                  std::void_t<typename M::index_type,
	                              decltype(M::rank()),
	                              decltype(M::rank_dynamic()),
	                              decltype(M::static_extent(std::declval<size_t>())),
	                              decltype(std::declval<M>().extent(std::declval<size_t>())),
	                              decltype(std::declval<M>().span_size()),
	                              decltype(std::declval<M>().stride(std::declval<size_t>())),
	                              decltype(M::is_always_unique),
	                              decltype(M::is_always_contiguous),
	                              decltype(M::is_always_strided),
	                              decltype(std::declval<M>().is_unique()),
	                              decltype(std::declval<M>().is_contiguous()),
	                              decltype(std::declval<M>().is_strided())>> : std::true_type
	{
	};

	template<typename M>
	constexpr bool is_mapping_v = is_mapping<M>::value;

	template<typename L, typename = void>
	struct is_layout : std::false_type
	{};

	template<typename L>
	struct is_layout<L, std::void_t<typename L::template mapping<extents<1>>>> : std::true_type
	{};

	template<typename L>
	constexpr bool is_layout_v = is_layout<L>::value;

	//template<typename T, typename Dimensions, typename Layout = layout_right>
	template<typename T, typename... Properties>
	struct mdspan
	{
		using extents_type = first_matching_type_t<is_extent, Properties...>;
		using layout_type  = first_matching_type_t<is_layout, Properties..., layout_right>;

		//using extents_type = Dimensions;
		//using layout_type  = Layout;
		using mapping_type = typename layout_type::template mapping<extents_type>;

		using my_type = mdspan<T, Properties...>;

		static_assert(is_extent_v<extents_type>, "Dimensions are ill-formed");
		static_assert(is_layout_v<layout_type>, "Layout is ill-formed");
		static_assert(is_mapping_v<mapping_type>, "Mapping is ill-formed");

		using element_type    = typename std::remove_all_extents_t<T>;
		using value_type      = typename std::remove_cv_t<element_type>;
		using index_type      = ptrdiff_t;
		using difference_type = ptrdiff_t;
		using pointer         = element_type*;
		using reference       = element_type&;

		constexpr mdspan() noexcept              = default;
		constexpr mdspan(mdspan&&) noexcept      = default;
		constexpr mdspan(mdspan const&) noexcept = default;
		~mdspan() noexcept                       = default;
		mdspan& operator=(mdspan&&) noexcept = default;
		mdspan& operator=(mdspan const&) noexcept = default;

		template<typename R, typename... RProperties>
		friend struct mdspan;

		static constexpr size_t rank() noexcept {
			return mapping_type::rank();
		}

		static constexpr size_t rank_dynamic() noexcept {
			return mapping_type::rank_dynamic();
		}

		static constexpr index_type static_extent(size_t e) noexcept {
			return mapping_type::static_extent(e);
		}

		template<typename R, typename... RProperties>
		static constexpr bool are_extents_compatible() {
			using other_type = mdspan<R, RProperties...>;
			for(size_t i = 0; i < my_type::rank(); ++i) {
				if(my_type::static_extent(i) != other_type::static_extent(i) && my_type::static_extent(i) != md::dynamic_extent) {
					return false;
				}
			}
			return true;
		}

		template<typename R, typename... RProperties, typename = std::enable_if_t<std::is_convertible_v<typename mdspan<R, RProperties...>::pointer, my_type::pointer>
		                                                                       && my_type::rank() == mdspan<R, RProperties...>::rank()
		                                                                       && are_extents_compatible<R, RProperties...>()>>
		constexpr mdspan(const mdspan<R, RProperties...>& rhs) noexcept : elements(rhs.elements) {
		}

		template<typename R, typename... RProperties, typename = std::enable_if_t<std::is_convertible_v<typename mdspan<R, RProperties...>::pointer, my_type::pointer>
		                                                                       && my_type::rank() == mdspan<R, RProperties...>::rank()
		                                                                       && are_extents_compatible<R, RProperties...>()>>
		mdspan& operator=(const mdspan<R, RProperties...>& rhs) noexcept {
			if(*this != rhs) {
				elements = rhs.elements;
			}
			return *this;
		}

		constexpr mdspan(std::nullptr_t) noexcept : elements() {
		}

		template<typename... IndexType>
		explicit constexpr mdspan(pointer elts, IndexType... DynamicExtents) noexcept : elements(elts), mapping(DynamicExtents...) {
		}

		template<typename... IndexType>
		explicit constexpr mdspan(span<element_type> elts, IndexType... DynamicExtents) noexcept : elements(elts.data()), mapping(DynamicExtents...) {
		}

		template<typename IndexType, size_t N>
		explicit constexpr mdspan(pointer elts, const std::array<IndexType, N>& dynamic_extents) noexcept : elements(elts), mapping(dynamic_extents) {
		}

		template<typename IndexType, size_t N>
		explicit constexpr mdspan(span<element_type> elts, const std::array<IndexType, N>& dynamic_extents) noexcept : elements(elts.data()), mapping(dynamic_extents) {
		}

		constexpr reference operator[](index_type idx) const noexcept {
			return elements[mapping(idx)];
		}

		template<typename IndexType, size_t N>
		constexpr reference operator[](const std::array<IndexType, N>& indices) {
			return this->operator()(indices);
		}

		template<typename... IndexType>
		constexpr reference operator()(IndexType... indices) const noexcept {
			return this->operator()(std::array<index_type, sizeof...(IndexType)>{ gsl::narrow<index_type>(indices)... });
		}

		template<typename IndexType, size_t N>
		constexpr reference operator()(const std::array<IndexType, N>& indices) const noexcept {
			return elements[mapping(indices)];
		}

		constexpr index_type extent(size_t e) const noexcept {
			return mapping.extent(e);
		}

		constexpr index_type size() const noexcept {
			return mapping.size();
		}

		constexpr md::span<element_type> span() const noexcept {
			return md::span<element_type>(elements, size());
		}

		template<typename... IndexType>
		static constexpr size_t required_span_size(IndexType... DynamicExtents) {
			return required_span_size(std::array<index_type, sizeof...(IndexType)>{ DynamicExtents... });
		}

		template<typename IndexType, size_t N>
		static constexpr size_t required_span_size(const std::array<IndexType, N>& dynamic_extents) {
			return mapping_type(dynamic_extents).span_size();
		}

		static constexpr bool is_always_unique     = mapping_type::is_always_unique;
		static constexpr bool is_always_contiguous = mapping_type::is_always_contiguous;
		static constexpr bool is_always_strided    = mapping_type::is_always_strided;

		constexpr bool is_unique() const {
			return mapping.is_unique();
		}
		constexpr bool is_contiguous() const {
			return mapping.is_contiguous();
		}
		constexpr bool is_strided() const {
			return mapping.is_strided();
		}

		constexpr index_type stride(size_t r) const {
			return mapping.stride(r);
		}

	//private:
		pointer elements;
		mapping_type mapping;
	};

	struct all_type_t
	{
	};

	constexpr all_type_t all = all_type_t{};

	template<typename S, typename = void>
	struct is_slice : std::false_type
	{
	};

	template<typename T>
	struct is_slice<std::initializer_list<T>, std::enable_if_t<std::is_integral_v<T>>> : std::true_type
	{
	};

	template<typename T>
	struct is_slice<std::pair<T, T>, std::enable_if_t<std::is_integral_v<T>>> : std::true_type
	{
	};

	template<typename T>
	struct is_slice<std::tuple<T, T>, std::enable_if_t<std::is_integral_v<T>>> : std::true_type
	{
	};

	template<typename T>
	struct is_slice<std::array<T, 2>, std::enable_if_t<std::is_integral_v<T>>> : std::true_type
	{
	};

	template<typename T>
	struct is_slice<T, std::enable_if_t<std::is_integral_v<T>>> : std::true_type
	{
	};

	template<>
	struct is_slice<all_type_t> : std::true_type
	{
	};

	template<typename T>
	constexpr bool is_slice_v = is_slice<T>::value;
}
