﻿#pragma once

// ui header
#include "ui_control.h"
#include "../style/ui_text.h"
#include "../core/ui_string.h"
#include "../text/ui_text_layout.h"
#include "../core/ui_const_sstring.h"
#include "../util/ui_named_control.h"
// cursor
#include <graphics/ui_cursor.h>

// ui namespace
namespace LongUI {
    // label
    class UILabel : public UIControl {
        // super class
        using Super = UIControl;
    protected:
        // ctor
        UILabel(UIControl* parent, const MetaControl&) noexcept;
    public:
        // class meta
        static const  MetaControl   s_meta;
        // dtor
        ~UILabel() noexcept;
        // ctor
        UILabel(UIControl* parent = nullptr) noexcept : UILabel(parent, UILabel::s_meta) {}
    public:
        // normal event
        auto DoEvent(UIControl*, const EventArg& e) noexcept->EventAccept override;
        // mouse event
        auto DoMouseEvent(const MouseEventArg& e) noexcept->EventAccept override;
        // update
        void Update() noexcept override;
        // render
        void Render() const noexcept override;
    protected:
        // add attribute
        void add_attribute(uint32_t key, U8View value) noexcept override;
#ifdef LUI_ACCESSIBLE
        // accessible api
        auto accessible(const AccessibleEventArg& args) noexcept->EventAccept override;
#endif
        // after set text
        void after_set_text() noexcept;
        // init label
        void reset_font() noexcept;
        // setup access key
        void setup_access_key() noexcept;
        // is default href
        auto is_def_href() const noexcept { return !m_href.empty() && m_oStyle.matched.empty(); }
    public:
        // get text
        auto GetText() const noexcept { return m_string.c_str(); }
        // get text- string object
        auto&GetTextString() const noexcept { return m_string; }
        // set text
        bool SetText(const CUIString& text) noexcept;
        // set text
        bool SetText(CUIString&& text) noexcept;
        // set text
        bool SetText(WcView text) noexcept;
        // set default minsize
        void SetAsDefaultMinsize() noexcept;
    public:
        // set connection control
        void SetControl(UIControl& ctrl) noexcept { m_control.SetControl(&ctrl); }
        // show access key
        void ShowAccessKey(bool show = true) noexcept;
    protected:
        // hovered curor
        CUICursor               m_hrefCursor;
        // connection control
        NamedControl            m_control;
        // text font buffer
        TextFont                m_tfBuffer;
        // href text
        CUIConstShortString     m_href;
        // text layout
        CUITextLayout           m_text;
        // text string
        CUIString               m_string;
        // access key position
        uint32_t                m_uPosAkey = 0;
    public:
        // default value
        enum : int32_t {
            // ** fonts are much differenty with each other
            // ** use this to adjust

            // default text x offset
            DEFUALT_TEXT_X_OFFSET = 0,
            // default text y offset
            DEFUALT_TEXT_Y_OFFSET = 0,
        };
    };
    // get meta info for UILabel
    LUI_DECLARE_METAINFO(UILabel);
}