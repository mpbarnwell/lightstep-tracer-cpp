#include "tracer/lightstep_span_context.h"

#include "common/hex_conversion.h"

namespace lightstep {
//--------------------------------------------------------------------------------------------------
// ToHexString
//--------------------------------------------------------------------------------------------------
static std::string ToHexString(uint64_t x) {
  std::array<char, Num64BitHexDigits> buffer;
  return Uint64ToHex(x, buffer.data());
}

//--------------------------------------------------------------------------------------------------
// ToTraceID
//--------------------------------------------------------------------------------------------------
//std::string LightStepSpanContext::ToTraceID() const noexcept try {
//  return ToHexString(this->trace_id());
//} catch (const std::exception& /*e*/) {
//  return {};
//}
//FIXME

//--------------------------------------------------------------------------------------------------
// ToSpanID
//--------------------------------------------------------------------------------------------------
std::string LightStepSpanContext::ToSpanID() const noexcept try {
  return ToHexString(this->span_id());
} catch (const std::exception& /*e*/) {
  return {};
}

//------------------------------------------------------------------------------
// operator==
//------------------------------------------------------------------------------
bool operator==(const LightStepSpanContext& lhs,
                const LightStepSpanContext& rhs) {
  auto extract_baggage = [](const LightStepSpanContext& span_context) {
    std::unordered_map<std::string, std::string> baggage;
    span_context.ForeachBaggageItem(
        [&](const std::string& key, const std::string& value) {
          baggage.emplace(key, value);
          return true;
        });
    return baggage;
  };

  return lhs.trace_id_high() == rhs.trace_id_high() &&
         lhs.trace_id_low() == rhs.trace_id_low() &&
         lhs.span_id() == rhs.span_id() &&
         lhs.trace_state() == rhs.trace_state() &&
         lhs.trace_flags() == rhs.trace_flags() &&
         extract_baggage(lhs) == extract_baggage(rhs);
}
}  // namespace lightstep
