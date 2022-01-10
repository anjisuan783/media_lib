/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include "rtc_base/struct_parameters_parser.h"

#include <algorithm>

#include "rtc_base/logging.h"

namespace webrtc {
namespace {
size_t FindOrEnd(std::string_view str, size_t start, char delimiter) {
  size_t pos = str.find(delimiter, start);
  pos = (pos == std::string::npos) ? str.length() : pos;
  return pos;
}
}  // namespace

namespace struct_parser_impl {
namespace {
inline void StringEncode(std::string* target, bool val) {
  *target += rtc::ToString(val);
}
inline void StringEncode(std::string* target, double val) {
  *target += rtc::ToString(val);
}
inline void StringEncode(std::string* target, int val) {
  *target += rtc::ToString(val);
}
inline void StringEncode(std::string* target, unsigned val) {
  *target += rtc::ToString(val);
}
inline void StringEncode(std::string* target, DataRate val) {
  *target += webrtc::ToString(val);
}
inline void StringEncode(std::string* target, DataSize val) {
  *target += webrtc::ToString(val);
}
inline void StringEncode(std::string* target, TimeDelta val) {
  *target += webrtc::ToString(val);
}

template <typename T>
inline void StringEncode(std::string* sb, std::optional<T> val) {
  if (val)
    StringEncode(sb, *val);
}
}  // namespace
template <typename T>
bool TypedParser<T>::Parse(std::string_view src, void* target) {
  auto parsed = ParseTypedParameter<T>(std::string(src));
  if (parsed.has_value())
    *reinterpret_cast<T*>(target) = *parsed;
  return parsed.has_value();
}
template <typename T>
void TypedParser<T>::Encode(const void* src, std::string* target) {
  StringEncode(target, *reinterpret_cast<const T*>(src));
}

template class TypedParser<bool>;
template class TypedParser<double>;
template class TypedParser<int>;
template class TypedParser<unsigned>;
template class TypedParser<std::optional<double>>;
template class TypedParser<std::optional<int>>;
template class TypedParser<std::optional<unsigned>>;

template class TypedParser<DataRate>;
template class TypedParser<DataSize>;
template class TypedParser<TimeDelta>;
template class TypedParser<std::optional<DataRate>>;
template class TypedParser<std::optional<DataSize>>;
template class TypedParser<std::optional<TimeDelta>>;
}  // namespace struct_parser_impl

StructParametersParser::StructParametersParser(
    std::vector<struct_parser_impl::MemberParameter> members)
    : members_(std::move(members)) {}

void StructParametersParser::Parse(std::string_view src) {
  size_t i = 0;
  while (i < src.length()) {
    size_t val_end = FindOrEnd(src, i, ',');
    size_t colon_pos = FindOrEnd(src, i, ':');
    size_t key_end = std::min(val_end, colon_pos);
    size_t val_begin = key_end + 1u;
    std::string_view key(src.substr(i, key_end - i));
    std::string_view opt_value;
    if (val_end >= val_begin)
      opt_value = src.substr(val_begin, val_end - val_begin);
    i = val_end + 1u;
    bool found = false;
    for (auto& member : members_) {
      if (key == member.key) {
        found = true;
        if (!member.parser.parse(opt_value, member.member_ptr)) {
          RTC_LOG(LS_WARNING) << "Failed to read field with key: '" << key
                              << "' in trial: \"" << src << "\"";
        }
        break;
      }
    }
    if (!found) {
      RTC_LOG(LS_INFO) << "No field with key: '" << key
                       << "' (found in trial: \"" << src << "\")";
    }
  }
}

std::string StructParametersParser::Encode() const {
  std::string res;
  bool first = true;
  for (const auto& member : members_) {
    if (!first)
      res += ",";
    res += member.key;
    res += ":";
    member.parser.encode(member.member_ptr, &res);
    first = false;
  }
  return res;
}

}  // namespace webrtc
