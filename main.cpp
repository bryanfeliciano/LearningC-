// using namespace std;

// #include <fstream>
// #include <vector>
// #include <stdexcept>
// #include <iostream>

// #pragma pack(1)
// struct FileHeader
// {
//     u16 file_type{ 0x4D42 };    // File type always BM which is 0x4D42
//     u32 file_size{ 0 };         // Size of the file (in bytes)
//     u16 reserved1{ 0 };         // Reserved, always 0
//     u16 reserved2{ 0 };         // Reserved, always 0
//     u32 offset_data{ 0 };       // Start position of pixel data
// };
// Include a readme.txt
// #pragma pack(1)
// struct InfoHeader
// {
//     u32 size{ 0 };              // Size of this header (in bytes)
//     s32 width{ 0 };             // width of bitmap in pixels
//     s32 height{ 0 };            // height of bitmap in pixels
//     u16 planes{ 1 };            // No. of planes, this is always 1
//     u16 bit_count{ 0 };         // No. of bits per pixel
//     u32 compression{ 0 };       // 0 or 3 - uncompressed.
//     u32 size_image{ 0 };        // 0 - for uncompressed images
//     s32 x_pixels_per_meter{ 0 };
//     s32 y_pixels_per_meter{ 0 };
//     u32 colors_used{ 0 };       // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
//     u32 colors_important{ 0 };  // No. of colors used for displaying the bitmap. If 0 all colors are required
// };

// #pragma pack(1)
// struct ColorHeader
// {
//     u32 RMask{ 0x00ff0000 };            // Bit mask for the red channel
//     u32 GMask{ 0x0000ff00 };            // Bit mask for the green channel
//     u32 BMask{ 0x000000ff };            // Bit mask for the blue channel
//     u32 AMask{ 0xff000000 };            // Bit mask for the alpha channel
//     u32 color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
//     u32 unused[16] { 0 };               // Unused data for sRGB color space
// };

// class BMP
// {
// private:
//     FileHeader file_header;
//     InfoHeader info_header;
//     ColorHeader color_header;
//     std::vector<u8> pixels;

//     u32 row_stride{ 0 };
// // &varName = reference to a pointer
//     void write_headers(std::ofstream &of)
//     {
//         of.write((const char*)&file_header, sizeof(file_header));
//         of.write((const char*)&info_header, sizeof(info_header));
//         if(info_header.bit_count == 32)
//         {
//             of.write((const char*)&color_header, sizeof(color_header));
//         }
//     }

//     void write_headers_and_data(std::ofstream &of)
//     {
//         write_headers(of);
//         of.write((const char*)pixels.data(), pixels.size());
//     }

//     // Add 1 to the row_stride until it is divisible with align_stride
//     u32 align_stride(u32 align_stride)
//     {
//         u32 new_stride = row_stride;
//         while (new_stride % align_stride != 0)
//         {
//             new_stride++;
//         }
//         return new_stride;
//     }

//     // Check if the pixel data is stored as BGRA and if the color space type is sRGB
//     void check_color_header(ColorHeader &color_header)
//     {
//         ColorHeader expected_color_header;
//         if( expected_color_header.RMask != color_header.RMask ||
//             expected_color_header.BMask != color_header.BMask ||
//             expected_color_header.GMask != color_header.GMask ||
//             expected_color_header.AMask != color_header.AMask)
//         {
//             throw std::runtime_error("Unexpected color mask format!\
//                 The program expects the pixel data to be in the BGRA format");
//         }
//         if(expected_color_header.color_space_type != color_header.color_space_type)
//         {
//             throw std::runtime_error("Unexpected color space type!\
//                 The program expects sRGB values");
//         }
//     }

// public:
//     BMP(s32 width, s32 height, bool has_alpha = true)
//     {
//         if (width <= 0 || height <= 0)
//         {
//             throw std::runtime_error("The image width and height must be positive numbers.");
//         }

//         info_header.width = width;
//         info_header.height = height;
//         if (has_alpha)
//         {
//             info_header.size = sizeof(InfoHeader) + 
//                                    sizeof(ColorHeader);
//             file_header.offset_data = sizeof(FileHeader) + 
//                                     sizeof(InfoHeader) + 
//                                     sizeof(ColorHeader);
// // remember offset data is the starting point of pixel data (default 0)
//             info_header.bit_count = 32;
//             info_header.compression = 3;
//             row_stride = width * 4;
//             pixels.resize(row_stride * height);
//             file_header.file_size = file_header.offset_data +
//                                         pixels.size();
//         }
//         else
//         {
//             info_header.size = sizeof(InfoHeader);
//             file_header.offset_data = sizeof(FileHeader) + 
//                                           sizeof(InfoHeader);

//             info_header.bit_count = 24;
//             info_header.compression = 0;
//             row_stride = width * 3;
//             pixels.resize(row_stride * height);

//             u32 new_stride = align_stride(4);
//             file_header.file_size = file_header.offset_data + 
//                                         static_cast<u32>(pixels.size()) + 
//                                         info_header.height * 
//                                         (new_stride - row_stride);
//         }
//     }
// // const meaning do not modify
//     BMP(const char *fname)
//     {
//         read(fname);
//     }

//     void read(const char *fname)
//     {
//         std::ifstream inp{ fname, std::ios_base::binary };
//         if (inp)
//         {
//             inp.read((char*)&file_header, sizeof(file_header));
//             if(file_header.file_type != 0x4D42)
//             {
//                 throw std::runtime_error("Error! Unrecognized file format.");
//             }
//             inp.read((char*)&info_header, sizeof(info_header));

//             // The ColorHeader is used only for transparent images
//             if(info_header.bit_count == 32)
//             {
//                 // Check if the file has bit mask color information
//                 if(info_header.size >= (sizeof(InfoHeader) + sizeof(ColorHeader)))
//                 {
//                     inp.read((char*)&color_header, sizeof(color_header));
//                     // Check if the pixel data is stored as BGRA and
//                     // if the color space type is sRGB
//                     check_color_header(color_header);
//                 }
//                 else
//                 {
//                     std::cerr << "Error! BMP file \"" << fname << "\" does not contain bit mask information\n";
//                     throw std::runtime_error("Error! Unrecognized file format.");
//                 }
//             }

//             // Jump to the pixel data location
//             inp.seekg(file_header.offset_data, inp.beg);

//             // Adjust the header fields for output.
//             // we only save the headers and the data.
//             if(info_header.bit_count == 32)
//             {
//                 info_header.size = sizeof(InfoHeader) + sizeof(ColorHeader);
//                 file_header.offset_data = sizeof(FileHeader) + sizeof(InfoHeader) + sizeof(ColorHeader);
//             }
//             else
//             {
//                 info_header.size = sizeof(InfoHeader);
//                 file_header.offset_data = sizeof(FileHeader) + sizeof(InfoHeader);
//             }
//             file_header.file_size = file_header.offset_data;

//             if (info_header.height < 0)
//             {
//                 throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
//             }

//             pixels.resize(info_header.width * info_header.height * info_header.bit_count / 8);

//             // check if we need to take row padding into account
//             if (info_header.width % 4 == 0)
//             {
//                 inp.read((char*)pixels.data(), pixels.size());
//                 file_header.file_size += static_cast<u32>(pixels.size());
//             }
//             else
//             {
//                 row_stride = info_header.width * info_header.bit_count / 8;
//                 u32 new_stride = align_stride(4);
//                 std::vector<u8> padding_row(new_stride - row_stride);

//                 for (int y = 0; y < info_header.height; ++y)
//                 {
//                     inp.read((char*)(pixels.data() + row_stride * y), row_stride);
//                     inp.read((char*)padding_row.data(), padding_row.size());
//                 }
//                 file_header.file_size += static_cast<u32>(pixels.size()) + 
//                                              info_header.height * 
//                                              static_cast<u32>(padding_row.size());
//             }
//         }
//         else
//         {
//             throw std::runtime_error("Unable to open the input image file.");
//         }
//     }

//     void write(const char *fname)
//     {
//         std::ofstream of{ fname, std::ios_base::binary };
//         if (of)
//         {
//             if (info_header.bit_count == 32)
//             {
//                 write_headers_and_data(of);
//             }
//             else if (info_header.bit_count == 24)
//             {
//                 if (info_header.width % 4 == 0)
//                 {
//                     write_headers_and_data(of);
//                 }
//                 else
//                 {
//                     u32 new_stride = align_stride(4);
//                     std::vector<u8> padding_row(new_stride - row_stride);

//                     write_headers(of);

//                     for (int y = 0; y < info_header.height; ++y)
//                     {
//                         of.write((const char*)(pixels.data() + row_stride * y), row_stride);
//                         of.write((const char*)padding_row.data(), padding_row.size());
//                     }
//                 }
//             }
//             else
//             {
//                 throw std::runtime_error("Only 24 or 32 bits per pixel BMP files are supported");
//             }
//         }
//         else
//         {
//             throw std::runtime_error("Unable to open the output image file.");
//         }
//     }

//     void negative()
//     {
//         u32 channels = info_header.bit_count / 8;
//         auto w = info_header.width;
//         auto h = info_header.height;

//         for (u32 y = 0; y < h; ++y)
//         {
//             for (u32 x = 0; x < w; ++x)
//             {
//                 u8 &B = pixels[channels * (y * w + x) + 0];
//                 u8 &G = pixels[channels * (y * w + x) + 1];
//                 u8 &R = pixels[channels * (y * w + x) + 2];
                
//                 B = 255 - B;
//                 G = 255 - G;
//                 R = 255 - R;

//                 B = B<0? 0: B;
//                 G = G<0? 0: G;
//                 R = R<0? 0: R;

//                 // if (channels == 4)
//                 // {
//                 //     u8 &A = pixels[channels * (y*w + x) + 3];
//                 //     A = 255 - A;
//                 //     A = A<0? 0: A;
//                 // }
//             }
//         }
//     }
// };

// #endif

// Create methods next 
// most basic data types
  // int age = 25 ;
  // float weigth = 75.2 ;
  // double balance = 100.00 ;
  // char gennder = 'M';
  // string username = "Bryan" ;
  // bool isTodaySunny = false ;
  // string colors[] = {"something", "like", "this"};
  // cout << colors[0];
  // cin >> colors[0];
  // cout << colors[0];
// we have to handle memory ourselves and be mindful of a var's byte size(that's weird)
// int main() {
//   int counter = 1 ;
//   do {
//     cout << counter << endl;
//     counter ++ ;
//   }while(counter<=10);
// }
// Instead of this you could use turnanry operators
  // if (isTodaySunny = true){
  //   cout << "Go to the park" << endl;
  // }
  // else {
  //   cout << "Stay home" << endl;
// turnary operator -->
  // isTodaySunny? cout << "Go to the park": cout << "Take an ummbrella";
// next we experimented with switch cases
// while
  // while(counter<=10){
  //   cout << counter << endl;
  //   counter ++ ;
  // }
// ~ = bitwise operator
//  color& denotes a const refernce 
// Std srgba structure, a channel negastion not required 