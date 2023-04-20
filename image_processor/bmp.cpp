#include "bmp.h"

namespace BMP {

template <typename T>
void ReadNum(std::ifstream& in, T& val) {
    uint8_t buff[sizeof(val)];
    in.read(reinterpret_cast<char*>(buff), sizeof(val));
    for (size_t i = 0; i < sizeof(val); i++) {
        val |= buff[i] << (8 * i);
    }
}

template <typename T>
T ReadNum(std::ifstream& in) {
    T val;
    ReadNum(in, val);
    return val;
}

template <typename T>
void WriteNum(std::ofstream& out, const T& val) {
    for (size_t i = 0; i < sizeof(val); ++i) {
        out.put(val >> (8 * i) & 0xFF);
    }
}

BMPHeader ReadBMPHeader(std::ifstream& in) {
    BMPHeader header;
    ReadNum(in, header.file_type);
    ReadNum(in, header.file_size);
    ReadNum(in, header.reserved1);
    ReadNum(in, header.reserved2);
    ReadNum(in, header.offset_data);
    return header;
}

DIBHeader ReadDIBHeader(std::ifstream& in) {
    DIBHeader dibheader;
    ReadNum(in, dibheader.header_size);
    ReadNum(in, dibheader.size.width);
    ReadNum(in, dibheader.size.height);
    ReadNum(in, dibheader.color_planes);
    ReadNum(in, dibheader.bits_per_pixel);
    ReadNum(in, dibheader.compression_method);
    ReadNum(in, dibheader.bitmap_data_size);
    ReadNum(in, dibheader.resolution.horizontal);
    ReadNum(in, dibheader.resolution.vertical);
    ReadNum(in, dibheader.colors.important);
    ReadNum(in, dibheader.colors.total);
    return dibheader;
}

void WriteBMPHeader(std::ofstream& out, const BMPHeader& header) {
    WriteNum(out, header.file_type);
    WriteNum(out, header.file_size);
    WriteNum(out, header.reserved1);
    WriteNum(out, header.reserved2);
    WriteNum(out, header.offset_data);
}

void WriteDIBHeader(std::ofstream& out, const DIBHeader& header) {
    WriteNum(out, header.header_size);
    WriteNum(out, header.size.width);
    WriteNum(out, header.size.height);
    WriteNum(out, header.color_planes);
    WriteNum(out, header.bits_per_pixel);
    WriteNum(out, header.compression_method);
    WriteNum(out, header.bitmap_data_size);
    WriteNum(out, header.resolution.horizontal);
    WriteNum(out, header.resolution.vertical);
    WriteNum(out, header.colors.important);
    WriteNum(out, header.colors.total);
}

Image LoadBMP(const std::string& path) {
    std::ifstream in{path, std::ios::binary};
    if (!in.is_open()) {
        throw CustomExceptions("Can not open input file");
    }
    BMPHeader header = ReadBMPHeader(in);
    if (header.file_type != FILE_TYPE) {
        throw CustomExceptions("Wrong input file type");
    }
    DIBHeader dib_header = ReadDIBHeader(in);
    if (dib_header.header_size != 40) {
        throw CustomExceptions("Wrong input file type");
    }
    Image image{dib_header.size.width, dib_header.size.height};
    for (auto i = 0; i < dib_header.size.height; ++i) {
        for (auto j = 0; j < dib_header.size.width; ++j) {
            Image::Pixel el;
            ReadNum(in, el.R);
            ReadNum(in, el.G);
            ReadNum(in, el.B);
            image.GetPixel(dib_header.size.height - i - 1, j) = el;
        }
        auto read_bytes = sizeof(Image::Pixel) * dib_header.size.width;
        if (read_bytes % 4 != 0) {
            in.ignore(4 - read_bytes % 4);
        }
    }
    return image;
}

void SaveBMP(const Image& image, const std::string& path) {
    try {
        BMPHeader bmpheader;
        bmpheader.file_type = 0x4d42;
        bmpheader.file_size = sizeof(BMPHeader) + sizeof(DIBHeader) + image.GetHeight() * image.GetWidth() * 3;
        bmpheader.offset_data = sizeof(BMPHeader) + sizeof(DIBHeader);

        DIBHeader dibheader;
        dibheader.size.width = image.GetWidth();
        dibheader.size.height = image.GetHeight();
        auto row_length = image.GetWidth() * dibheader.bits_per_pixel;
        auto padding = ((row_length + 3) / STR_SEPARATOR) * STR_SEPARATOR - row_length;
        dibheader.bitmap_data_size = image.GetHeight() * (row_length + padding);

        std::ofstream out{path, std::ofstream::binary};
        WriteBMPHeader(out, bmpheader);
        WriteDIBHeader(out, dibheader);
        for (int i = static_cast<size_t>(image.GetHeight() - 1); i >= 0; --i) {
            for (size_t j = 0; j < image.GetWidth(); ++j) {
                WriteNum(out, image.GetPixel(i, j).R);
                WriteNum(out, image.GetPixel(i, j).G);
                WriteNum(out, image.GetPixel(i, j).B);
            }
            for (size_t j = 0; j < padding; ++j) {
                WriteNum<char>(out, 0);
            }
        }
        out.close();
    } catch (const std::exception& e) {
        throw CustomExceptions("Error with saving output file");
    }
}
}  // namespace BMP
