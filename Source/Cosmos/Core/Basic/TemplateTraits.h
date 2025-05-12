#pragma once

namespace TemplateTraits {
	template<typename Tuple, std::size_t... Indices>
	Tuple ParseParamsFromBuffer(void* ParamsBuffer, std::index_sequence<Indices...>) {
		char* Buffer = static_cast<char*>(ParamsBuffer);
		return Tuple{ (*reinterpret_cast<typename std::tuple_element<Indices, Tuple>::type*>(
			Buffer + (Indices * sizeof(typename std::tuple_element<Indices, Tuple>::type))))
			... };
	}

	template<typename... ArgsType>
	std::vector<uint8_t> BuildParamsBuffer(ArgsType... Args) {
		if constexpr (sizeof...(Args) == 0) {
			return std::vector<uint8_t>{};
		}
		else {
			size_t TotalSize = (sizeof(Args) + ...);
			std::vector<uint8_t> Buffer(TotalSize);
			size_t Offset = 0;
			([&Buffer, &Offset](auto& Arg) {
				std::memcpy(Buffer.data() + Offset, &Arg, sizeof(Arg));
				Offset += sizeof(Arg);
				}(Args), ...);

			return Buffer;
		}
	}

	template<typename T>
	struct FuncTraits;

	template<typename Ret, typename... Args>
	struct FuncTraits<Ret(Args...)> {
		using TupleType = std::tuple<Args...>;
	};

	template<typename Ret, typename... Args>
	struct FuncTraits<Ret(*)(Args...)> : FuncTraits<Ret(Args...)> {};

	template<typename Ret, typename... Args>
	struct FuncTraits<TFunction<Ret(Args...)>> : FuncTraits<Ret(Args...)> {};

	template<typename Lambda>
	struct FuncTraits : FuncTraits<decltype(&Lambda::operator())> {};

	template<typename ClassType, typename Ret, typename... Args>
	struct FuncTraits<Ret(ClassType::*)(Args...) const> : FuncTraits<Ret(Args...)> {};
};
