#pragma once

#include "config.hpp"
#include <array>
#include <vector>
#include <queue>
#include <sstream>

namespace glshader::process::compress::huffman
{
    struct stream {
        size_t stream_length;
        std::basic_stringstream<uint8_t> stringstream;

        template<typename Container, typename BaseContainer = std::decay_t<std::remove_const_t<Container>>,
            typename = std::void_t<
                typename BaseContainer::value_type, 
                decltype(std::declval<BaseContainer>().resize(size_t(0))),
                decltype(std::size(std::declval<BaseContainer>())), 
                decltype(std::data(std::declval<BaseContainer>()))
            >
        >
        BaseContainer to_container()
        {
            BaseContainer container;
            container.resize(stream_length / sizeof(typename BaseContainer::value_type));
            stringstream.read(reinterpret_cast<uint8_t*>(std::data(container)), std::size(container) * sizeof(typename BaseContainer::value_type));
            return container;
        }
    };

    stream encode(const std::basic_string<uint8_t>& in);
    stream encode(const std::vector<uint8_t>& in);
    stream encode(const uint8_t* in, size_t in_length);
    
    stream decode(const std::basic_string<uint8_t>& in);
    stream decode(const std::vector<uint8_t>& in);
    stream decode(const uint8_t* in, size_t in_length);
}