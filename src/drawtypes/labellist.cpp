#include "drawtypes/labellist.hpp"
#include "utils/factory.hpp"

POLYBAR_NS

namespace drawtypes {
  label_t& labellist::get_template() {
    return m_template;
  }

  void labellist::apply_template() {
    for (label_t& label : m_labels) {
      label->useas_token(m_template, "%label%");
    }
  }

  void labellist::apply_gradient(label_t& label, float percentage) {
    if (m_foreground_grad) {
      label->m_foreground = m_foreground_grad->get_by_percentage(percentage);
    }
    if (m_background_grad) {
      label->m_background = m_background_grad->get_by_percentage(percentage);
    }
    if (m_underline_grad) {
      label->m_underline = m_underline_grad->get_by_percentage(percentage);
    }
    if (m_overline_grad) {
      label->m_overline = m_overline_grad->get_by_percentage(percentage);
    }
  }

  
  void load_labellist(vector<label_t>& labels, label_t& tmplate,
      gradient_t& foreground_grad,
      gradient_t& background_grad,
      gradient_t& underline_grad,
      gradient_t& overline_grad,
      const config& conf, const string& section, string name, bool required) {
    name = string_util::ltrim(string_util::rtrim(move(name), '>'), '<');

    vector<string> names;
    if(required) {
      names = conf.get_list(section, name);
    } else {
      names = conf.get_list(section, name, {});
    }

    if (conf.get(section, name + "-foreground-gradient", false)) {
      foreground_grad = factory_util::shared<gradient>();
    }
    if (conf.get(section, name + "-background-gradient", false)) {
      background_grad = factory_util::shared<gradient>();
    }
    if (conf.get(section, name + "-underline-gradient", false)) {
      underline_grad = factory_util::shared<gradient>();
    }
    if (conf.get(section, name + "-overline-gradient", false)) {
      overline_grad = factory_util::shared<gradient>();
    }

    tmplate = load_label(conf, section, name, false, "%label%");
    for (size_t i = 0; i < names.size(); i++) {
      labels.emplace_back(forward<label_t>(load_optional_label(conf, section, name + "-" + to_string(i), names[i])));
      labels.back()->copy_undefined(tmplate);
      labels.back()->useas_token(tmplate, "%label%");
      if (foreground_grad) {
        foreground_grad->add(hsla::get_hsla(labels.back()->m_foreground));
      }
      if (background_grad) {
        background_grad->add(hsla::get_hsla(labels.back()->m_background));
      }
      if (underline_grad) {
        underline_grad->add(hsla::get_hsla(labels.back()->m_underline));
      }
      if (overline_grad) {
        overline_grad->add(hsla::get_hsla(labels.back()->m_overline));
      }
    }
  }
}

POLYBAR_NS_END