// Copyright 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>

#include "gmock/gmock.h"
using ::testing::Eq;
using ::testing::Ne;

GTEST_API_ int
main(int argc, char **argv)
{
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class Soundex
{
public:
  std::string
  encode(const std::string& word) const
  {
    return word;
  }
};

TEST(SoundexEncodingClass, RetainsSoleLetterOfOneLetterWord)
{
  Soundex soundex;

  auto encoded = soundex.encode("A");
  EXPECT_THAT(encoded, Eq("A")) << "must be equal to A";
  EXPECT_THAT(encoded, Ne("A")) << "must not be equal to A";
  ASSERT_THAT(encoded, Eq("A"));
}

TEST(EqualsClass, CheckSimpleConditions)
{
  int i=7;

  EXPECT_EQ(8, i);
  EXPECT_EQ(i, 8);

  EXPECT_EQ(7, i) << "7 must be equal to i";
  EXPECT_NE(7, i);
  EXPECT_GE(7, i);

  EXPECT_TRUE(i==7) << "i==7 must be true";
  EXPECT_FALSE(i==7) << "i==7 must be false";
}


/* https://code.google.com/p/googletest/wiki/Documentation */
/* https://code.google.com/p/googlemock/wiki/Documentation */

