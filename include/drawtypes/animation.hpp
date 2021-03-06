#pragma once

#include <atomic>
#include <chrono>

#include "common.hpp"
#include "components/config.hpp"
#include "drawtypes/label.hpp"
#include "drawtypes/labellist.hpp"
#include "utils/mixins.hpp"

POLYBAR_NS

namespace chrono = std::chrono;

namespace drawtypes {
  class animation : public labellist {
   public:
    explicit animation(unsigned int framerate_ms) : m_framerate_ms(framerate_ms) {}
    explicit animation(vector<label_t>&& frames, int framerate_ms, size_t subframecount, label_t&& tmplate,
          gradient_t&& fg,
          gradient_t&& bg,
          gradient_t&& ul,
          gradient_t&& ol)
        : labellist(move(frames), move(tmplate), move(fg), move(bg), move(ul), move(ol))
        , m_framerate_ms(framerate_ms)
        , m_subframecount(subframecount)
        , m_framecount(m_labels.size())
        , m_frame(m_labels.size() - 1) {}

    void add(label_t&& frame);
    void increment();

    label_t get();
    unsigned int framerate() const;

    explicit operator bool() const;

   protected:
    unsigned int m_framerate_ms = 1000;
    size_t m_subframecount = 0;
    size_t m_framecount = 0;
    std::atomic_size_t m_frame{0_z};
    std::atomic_size_t m_subframe{0_z};
  };

  using animation_t = shared_ptr<animation>;

  animation_t load_animation(
      const config& conf, const string& section, string name = "animation", bool required = true);
}  // namespace drawtypes

POLYBAR_NS_END
