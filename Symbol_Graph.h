#pragma once

#include <stdexcept>
#include <string_view>
#include <functional>
#include <concepts>

template<typename Key_T, typename Value_T, std::integral Bias_T = std::uint8_t>
class Symbol_Graph {
public:
	class Entry;

	using This = Symbol_Graph;
	using Key_Type = Key_T;
	using Value_Type = Value_T;
	using Bias_Type = Bias_T;
	using Can_Be_Biasless_Fn = bool (*)(Entry const&);

public:
	class Entry {
		friend class Symbol_Graph;

	public:
		using This = Entry;
	
	public:
		Entry() = default;
		constexpr Entry(Key_Type&& key, Value_Type& value, Bias_Type bias) noexcept
			: key_(key), value_(&value), bias_(bias) {}
		
		~Entry();
	
	public:
		constexpr Key_Type const& key() const noexcept { return this->key_; }
		constexpr Value_Type const& value() const noexcept { return this->value_; }

		constexpr This const* parent() const noexcept { return this->parent_; }
		constexpr This const* child() const noexcept { return this->child_; }

		constexpr This const* prior() const noexcept { return this->prior_; }
		constexpr This const* next() const noexcept { return this->next_; }

		constexpr Bias_Type bias() const noexcept { return this->bias_; }
	
	private:
		Key_Type key_;
		Value_Type& value_;
		This* parent_ = nullptr;
		This* child_ = nullptr;
		This* prior_ = nullptr;
		This* next_ = nullptr;
		Bias_Type bias_;
	
	private:
		void append(This& next) noexcept;
		void prepend(This& prior) noexcept;
		void detach() noexcept;

		void increase_bias() noexcept;
		void decrease_bias() noexcept;
	};

public:
	constexpr Symbol_Graph(Can_Be_Biasless_Fn fn) noexcept
		: can_be_biasless(fn) {}

	~Symbol_Graph();

public:
	void enter(Entry& entry, Entry& parent, bool is_super = false);
	
	Value_Type& get_from(Key_Type const& key, Entry const& parent);

private:
	Entry* root_ = nullptr;
	Entry* current_ = nullptr;
	Entry* super_ = nullptr;

public:
	Can_Be_Biasless_Fn can_be_biasless;
};
