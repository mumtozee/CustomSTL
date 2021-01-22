#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <valarray>
#include <vector>

struct WavHeader {
  char chunk_id[4];
  uint32_t chunk_size;
  char format[4];
  char subchunk1_id[4];
  uint32_t subchunk1_size;
  uint16_t audio_format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;
  char subchunk2_id[4];
  uint32_t subchunk2_size;
};

template <typename T>
void PrintArray(const T* arr, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    std::cout << arr[i];
  }
}

void PrintHeader(const WavHeader& header) {
  std::cout << "RIFF: ";
  PrintArray(header.chunk_id, 4);
  std::cout << '\n';
  std::cout << "Overall Size: " << header.chunk_size << '\n';
  std::cout << "WAVE: ";
  PrintArray(header.format, 4);
  std::cout << '\n';
  std::cout << "FMT: ";
  PrintArray(header.subchunk1_id, 4);
  std::cout << '\n';
  std::cout << "Length of FMT: " << header.subchunk1_size << '\n';
  std::cout << "Format Type: " << header.audio_format << '\n';
  std::cout << "Channels: " << header.num_channels << '\n';
  std::cout << "Sample rate: " << header.sample_rate << '\n';
  std::cout << "Byte rate: " << header.byte_rate << '\n';
  std::cout << "Block align: " << header.block_align << '\n';
  std::cout << "Bits per sample: " << header.bits_per_sample << '\n';
  std::cout << "Data chunk reader: ";
  PrintArray(header.subchunk2_id, 4);
  std::cout << '\n';
  std::cout << "Data size: " << header.subchunk2_size << '\n';
}

using Complex = std::complex<double>;

uint32_t RoundUpToPowerofTwo(uint32_t input) {
  --input;
  input |= input >> 1;
  input |= input >> 2;
  input |= input >> 4;
  input |= input >> 8;
  input |= input >> 16;
  ++input;
  return input;
}

void FFT(std::valarray<Complex>& data) {
  if (data.size() <= 1) {
    return;
  }
  std::valarray<Complex> even = data[std::slice(0, data.size() / 2, 2)];
  std::valarray<Complex> odd = data[std::slice(1, data.size() / 2, 2)];
  FFT(even);
  FFT(odd);
  for (uint32_t i = 0; i < data.size() / 2; ++i) {
    Complex temp = std::polar(1.0, -2 * M_PI * i / data.size()) * odd[i];
    data[i] = even[i] + temp;
    data[i + data.size() / 2] = even[i] - temp;
  }
}

void FFTReverse(std::valarray<Complex>& data) {
  data = data.apply(std::conj);
  FFT(data);
  data = data.apply(std::conj);
  data /= data.size();
}

void AnnihilateLastCoeffs(std::valarray<Complex>& data, size_t pure_size) {
  uint32_t percent = static_cast<double>(pure_size) * 0.2;
  std::fill(std::begin(data) + percent, std::end(data), Complex{0.0, 0.0});
}

template <typename T>
void ResizeValarray(std::valarray<T>& varr, size_t new_size, T value = T()) {
  if (new_size == varr.size()) {
    return;
  }
  std::vector<T> buffer(std::begin(varr), std::end(varr));
  buffer.resize(new_size, T());
  varr.resize(new_size);
  std::copy(buffer.begin(), buffer.end(), std::begin(varr));
}

int main() {
  std::ifstream in{"speech.wav", std::ios::in | std::ios::binary};
  WavHeader head{};
  in.read(reinterpret_cast<char*>(&head), sizeof(WavHeader));
  PrintHeader(head);
  std::cout << sizeof(head) << '\n';

  const size_t data_size = head.subchunk2_size / 2;
  auto* data = new int16_t[data_size];
  in.read(reinterpret_cast<char*>(data), data_size * sizeof(uint16_t));
  in.close();
  auto* complex_data = new Complex[data_size];
  std::copy(data, data + data_size, complex_data);
  std::valarray<Complex> carray(complex_data, data_size);
  delete[] complex_data;
  delete[] data;

  auto original_size = carray.size();
  ResizeValarray(carray, RoundUpToPowerofTwo(original_size), Complex{0.0, 0.0});
  FFTReverse(carray);
  AnnihilateLastCoeffs(carray, original_size);
  FFT(carray);
  ResizeValarray(carray, original_size);

  std::ofstream out{"out.wav", std::ios::out | std::ios::binary};
  auto* out_data = new int16_t[original_size];
  uint32_t i = 0;
  std::for_each(
      out_data, out_data + original_size,
      [&i, &carray](int16_t& number) { number = round(carray[i++].real()); });
  out.write(reinterpret_cast<char*>(&head), sizeof(WavHeader));
  out.write(reinterpret_cast<char*>(out_data), head.subchunk2_size);
  delete[] out_data;
  out.close();
  return 0;
}
