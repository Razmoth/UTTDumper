#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

namespace UTTD {
	class MD4 {
	public:
		MD4() {
			reset();
		}

		void reset() {
			count = 0;
			context.assign(4, 0);
			context[0] = 0x67452301;
			context[1] = 0xefcdab89;
			context[2] = 0x98badcfe;
			context[3] = 0x10325476;
			x.assign(16, 0);
			buffer.assign(BLOCK_LENGTH, 0);
		}

		template <typename T>
		void update(T const& value) {
			update((uint8_t*)&value, 0, sizeof(T));
		}

		void update(const std::vector<uint8_t>& vector) {
			update(vector.data(), 0, vector.size());
		}


		void update(const uint8_t* data, int32_t offset, int32_t size) {
			if (offset < 0 || size < 0 || offset > size) {
				throw std::invalid_argument("Incorrect arguments for function engine_update");
			}

			int32_t buffer_index = static_cast<int32_t>(count % BLOCK_LENGTH);
			count += size;
			const int32_t partial_length = BLOCK_LENGTH - buffer_index;
			int32_t i = 0;

			if (size >= partial_length) {
				for (int32_t j = 0; j < partial_length; ++j) {
					buffer[buffer_index + j] = data[offset + j];
				}

				transform(buffer.data(), 0);
				i = partial_length;
				while (i + BLOCK_LENGTH - 1 < size) {
					transform(data, offset + i);
					i += BLOCK_LENGTH;
				}

				buffer_index = 0;
			}

			if (i < size) {
				for (int32_t j = 0; j < size - i; ++j) {
					buffer[buffer_index + j] = data[offset + i + j];
				}
			}
		}

		std::vector<uint8_t> digest() {
			const int32_t buffer_index = static_cast<int32_t>(count % BLOCK_LENGTH);
			const int32_t padding_length = (buffer_index < 56) ? 56 - buffer_index : 120 - buffer_index;

			std::vector<uint8_t> tail(padding_length + 8, 0);
			tail[0] = static_cast<uint8_t>(0x80);

			for (int32_t i = 0; i < 8; ++i) {
				tail[padding_length + i] = static_cast<uint8_t>(unsigned_right_shift(count * 8, 8 * i));
			}

			update(tail);

			std::vector<uint8_t> result(16, 0);
			for (int32_t i = 0; i < 4; ++i) {
				for (int32_t j = 0; j < 4; ++j) {
					result[i * 4 + j] = static_cast<uint8_t>(unsigned_right_shift(context[i], 8 * j));
				}
			}

			reset();
			return result;
		}

	private:
		void transform(const uint8_t* data, int32_t offset) {
			for (int32_t i = 0; i < 16; ++i) {
				x[i] = ((data[offset + 0] & 0xff)) |
					((data[offset + 1] & 0xff) << 8) |
					((data[offset + 2] & 0xff) << 16) |
					((data[offset + 3] & 0xff) << 24);
				offset += 4;
			}

			uint32_t a = context[0];
			uint32_t b = context[1];
			uint32_t c = context[2];
			uint32_t d = context[3];

			for (const int32_t& i : { 0, 4, 8, 12 }) {
				a = ff(a, b, c, d, x[i + 0], 3);
				d = ff(d, a, b, c, x[i + 1], 7);
				c = ff(c, d, a, b, x[i + 2], 11);
				b = ff(b, c, d, a, x[i + 3], 19);
			}

			for (const int32_t& i : { 0, 1, 2, 3 }) {
				a = gg(a, b, c, d, x[i + 0], 3);
				d = gg(d, a, b, c, x[i + 4], 5);
				c = gg(c, d, a, b, x[i + 8], 9);
				b = gg(b, c, d, a, x[i + 12], 13);
			}

			for (const int32_t& i : { 0, 2, 1, 3 }) {
				a = hh(a, b, c, d, x[i + 0], 3);
				d = hh(d, a, b, c, x[i + 8], 9);
				c = hh(c, d, a, b, x[i + 4], 11);
				b = hh(b, c, d, a, x[i + 12], 15);
			}

			context[0] += a;
			context[1] += b;
			context[2] += c;
			context[3] += d;
		}

		static uint32_t unsigned_right_shift(const uint32_t& base, const uint32_t& shift) {
			if (shift < 0 || shift >= 32 || base == 0) {
				return 0;
			}

			return (base > 0) ? base >> shift : static_cast<uint32_t>(base) >> shift;
		}

		static uint32_t rotate(const uint32_t& t, const uint32_t& s) {
			return (t << s) | unsigned_right_shift(t, 32 - s);
		}

		static uint32_t ff(const uint32_t& a, const uint32_t& b, const uint32_t& c, const uint32_t& d, const uint32_t& x, const uint32_t& s) {
			return rotate(a + ((b & c) | (~b & d)) + x, s);
		}

		static uint32_t gg(const uint32_t& a, const uint32_t& b, const uint32_t& c, const uint32_t& d, const uint32_t& x, const uint32_t& s) {
			return rotate(a + ((b & (c | d)) | (c & d)) + x + 0x5A827999, s);
		}

		static uint32_t hh(const uint32_t& a, const uint32_t& b, const uint32_t& c, const uint32_t& d, const uint32_t& x, const uint32_t& s) {
			return rotate(a + (b ^ c ^ d) + x + 0x6ED9EBA1, s);
		}

		uint64_t count;
		std::vector<uint32_t> x;
		std::vector<uint32_t> context;
		std::vector<uint8_t> buffer;

		const int32_t BLOCK_LENGTH = 64;
	};
}