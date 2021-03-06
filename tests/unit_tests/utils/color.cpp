#include "common/test.hpp"
#include "utils/color.hpp"

using namespace polybar;

TEST(String, rgb) {
  unsigned int color{0x123456};
  EXPECT_EQ(0, color_util::alpha_channel<unsigned char>(color));
  EXPECT_EQ(0x12, color_util::red_channel<unsigned char>(color));
  EXPECT_EQ(0x34, color_util::green_channel<unsigned char>(color));
  EXPECT_EQ(0x3434, color_util::green_channel<unsigned short int>(color));
  EXPECT_EQ(0x56, color_util::blue_channel<unsigned char>(color));

  EXPECT_TRUE((0x33 + 0.5) / 256.0 == rgb{0xFF112233}.b);
  EXPECT_TRUE((0x51 + 0.5) / 256.0 == rgb{0x88449933}.g);
  EXPECT_TRUE(0xff0f0f0f == rgb{0xee111111});
  EXPECT_TRUE(0xff0a141e == rgb{0x99112233});
}

TEST(String, rgba) {
  unsigned int color{0xCC123456};
  EXPECT_EQ(0xCCCC, color_util::alpha_channel<unsigned short int>(color));
  EXPECT_EQ(0x1212, color_util::red_channel<unsigned short int>(color));
  EXPECT_EQ(0x12, color_util::red_channel<unsigned char>(color));
  EXPECT_EQ(0x3434, color_util::green_channel<unsigned short int>(color));
  EXPECT_EQ(0x5656, color_util::blue_channel<unsigned short int>(color));

  EXPECT_EQ((0xCC + 0.5) / 256.0, rgba{0xCC112233}.a);
  EXPECT_EQ((0x99 + 0.5) / 256.0, rgba{0x88449933}.g);
  EXPECT_EQ(0xFF111111, static_cast<unsigned int>(rgba{0xFF111111}));
  EXPECT_EQ(0x00FFFFFF, static_cast<unsigned int>(rgba{0x00FFFFFF}));
}

TEST(String, hex) {
  unsigned int colorA{0x123456};
  EXPECT_EQ("#123456"s, color_util::hex<unsigned char>(colorA));
  unsigned int colorB{0xCC123456};
  EXPECT_EQ("#cc123456"s, color_util::hex<unsigned short int>(colorB));
  unsigned int colorC{0x00ffffff};
  EXPECT_EQ("#00ffffff"s, color_util::hex<unsigned short int>(colorC));
}

TEST(String, parseHex) {
  EXPECT_EQ("#ffffffff", color_util::parse_hex("#fff"));
  EXPECT_EQ("#ff112233", color_util::parse_hex("#123"));
  EXPECT_EQ("#ff888888", color_util::parse_hex("#888888"));
  EXPECT_EQ("#00aa00aa", color_util::parse_hex("#00aa00aa"));
}

TEST(String, parse) {
  EXPECT_EQ(0, color_util::parse("invalid"));
  EXPECT_EQ(0, color_util::parse("#f"));
  EXPECT_EQ(0, color_util::parse("#ff"));
  EXPECT_EQ(0xFF999999, color_util::parse("invalid", 0xFF999999));
  EXPECT_EQ(0x00111111, color_util::parse("invalid", 0x00111111));
  EXPECT_EQ(0xFF000000, color_util::parse("invalid", 0xFF000000));
  EXPECT_EQ(0xffffffff, color_util::parse("#fff"));
  EXPECT_EQ(0xFF889900, color_util::parse("#890"));
  EXPECT_EQ(0x55888777, color_util::parse("#55888777"));
  EXPECT_EQ(0x88aaaaaa, color_util::parse("#88aaaaaa"));
  EXPECT_EQ(0x00aaaaaa, color_util::parse("#00aaaaaa"));
  EXPECT_EQ(0x00FFFFFF, color_util::parse("#00FFFFFF"));
}

TEST(String, simplify) {
  EXPECT_EQ("#111", color_util::simplify_hex("#FF111111"));
  EXPECT_EQ("#234", color_util::simplify_hex("#ff223344"));
  EXPECT_EQ("#ee223344", color_util::simplify_hex("#ee223344"));
  EXPECT_EQ("#234567", color_util::simplify_hex("#ff234567"));
  EXPECT_EQ("#00223344", color_util::simplify_hex("#00223344"));
}

TEST(Color, Parse) {
   EXPECT_EQ(0xffff0000, color_util::parse("#FF0000"));
   EXPECT_EQ(0xffff8080, color_util::parse("hsl(0, 1, 0.75, 1)"));
   EXPECT_EQ(0xff00ffc0, color_util::parse("rgb(0, 1, 0.75)"));
   EXPECT_EQ(0x80ff8080, color_util::parse("hsl(0, 1, 0.75, 0.5)"));
   EXPECT_EQ(0x80ffffff, color_util::parse("jzazbz(0.22076484311386071, -0.00016356327300187656, -0.00010207018471919405, 0.5)"));
}

template <typename ValueType>
ValueType delta(ValueType one, ValueType two) {
  return one > two ? one - two : two - one;
}
//TEST(String, hsl) {
//  // Value of each channel can deviate from the expected value by 1
//  EXPECT_EQ(delta(0xff000000, static_cast<unsigned int>(hsl(360, 0, 0).to_rgb())) & 0xfffefefe, 0);
//  EXPECT_EQ(delta(0xff00ff00, static_cast<unsigned int>(hsl(120, 1, 0.5).to_rgb())) & 0xfffefefe, 0);
//  EXPECT_EQ(delta(0xff0000ff, static_cast<unsigned int>(hsl(240, 1, 0.5).to_rgb())) & 0xfffefefe, 0);
//  EXPECT_EQ(delta(0xff00ffff, static_cast<unsigned int>(hsl(180, 1, 0.5).to_rgb())) & 0xfffefefe, 0);
//  EXPECT_EQ(delta(0xff008888, static_cast<unsigned int>(hsl::from_rgb(rgb{0xff008888}).to_rgb())) & 0xfffefefe, 0);
//  EXPECT_EQ(delta(0xff008800, static_cast<unsigned int>(hsl::from_rgb(rgb{0xff008800}).to_rgb())) & 0xfffefefe, 0);
//  EXPECT_EQ(delta(0xff123456, static_cast<unsigned int>(hsl::from_rgb(rgb{0xff123456}).to_rgb())) & 0xfffefefe, 0);
//  EXPECT_EQ(0.5, hsl(240, 1, 0.25).to_rgb().b);
//  EXPECT_EQ(1.0, hsl(240, 1, 0.75).to_rgb().b);
//  EXPECT_EQ(0.5, hsl(240, 1, 0.75).to_rgb().r);
//  EXPECT_EQ(hsl(240, 1, 0.75).to_rgba(0.75).r, hsl(240, 1, 0.75).to_rgb().r);
//  EXPECT_EQ(hsl(240, 1, 0.75).to_rgba(0.75).g, hsl(240, 1, 0.75).to_rgb().g);
//  EXPECT_EQ(hsl(240, 1, 0.75).to_rgba(0.75).b, hsl(240, 1, 0.75).to_rgb().b);
//  EXPECT_EQ(0.5, hsl(240, 1, 0.75).to_rgba(0.75).g);
//}
