﻿// Gui
#include <core/ui_ctrlmeta.h>
#include <debugger/ui_debug.h>
#include <control/ui_checkbox.h>
// 子控件
#include <control/ui_boxlayout.h>
#include <core/ui_unsafe.h>
#include <constexpr/const_bkdr.h>
// Private
#include "../private/ui_private_control.h"

// ui namespace
namespace LongUI {
    // UICheckBox类 元信息
    LUI_CONTROL_META_INFO(UICheckBox, "checkbox");
}

PCN_NOINLINE
/// <summary>
/// Sets the indeterminate.
/// </summary>
/// <returns></returns>
void LongUI::UICheckBox::SetIndeterminate() noexcept {
    // 禁用状态
    if (this->IsDisabled()) return;
    // 允许修改
    if (!this->GetIndeterminate()) {
        this->change_indeterminate(true);
        this->changed();
    }
}

/// <summary>
/// Sets the image source.
/// </summary>
/// <param name="src">The source.</param>
/// <returns></returns>
void LongUI::UICheckBox::SetImageSource(U8View src) noexcept {
    if (!m_pImageChild) {
        const auto img = new(std::nothrow) UIImage{ this };
        if (!img) return;
        Super::SwapChildren(*img, m_oLabel);
        m_pImageChild = img;
    }
    m_pImageChild->SetSource(src);
}

PCN_NOINLINE
/// <summary>
/// Sets the checked.
/// </summary>
/// <param name="checked">if set to <c>true</c> [checked].</param>
/// <returns></returns>
void LongUI::UICheckBox::SetChecked(bool checked) noexcept {
    // 禁用状态
    if (this->IsDisabled()) return;
    bool changed_flag = false;
    // 是第三方状态?
    if (this->GetIndeterminate()) {
        this->change_indeterminate(false);
        changed_flag = true;
    }
    // 是相反状态?
    if (this->GetChecked() != checked) {
        changed_flag = true;
        const auto statetp = StyleStateType::Type_Checked;
        this->StartAnimation({ statetp , checked });
        m_oImage.StartAnimation({ statetp , checked });
    }
    // 修改状态
    if (changed_flag) this->changed();
}

/// <summary>
/// Sets the indeterminate.
/// </summary>
/// <param name="">if set to <c>true</c> [].</param>
/// <returns></returns>
void LongUI::UICheckBox::change_indeterminate(bool ndeterminate) noexcept {
    assert(this->GetIndeterminate() != ndeterminate);
    const auto statetp = StyleStateType::Type_Indeterminate;
    this->StartAnimation({ statetp , ndeterminate });
    m_oImage.StartAnimation({ statetp , ndeterminate });
}

/// <summary>
/// Changes this instance.
/// </summary>
/// <returns></returns>
void LongUI::UICheckBox::changed() noexcept {
    this->TriggerEvent(this->_onCommand());
    // TODO: ACCESSIBLE
#ifndef LUI_ACCESSIBLE

#endif
}

/// <summary>
/// Initializes a new instance of the <see cref="UICheckBox" /> class.
/// </summary>
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UICheckBox::UICheckBox(UIControl* parent, const MetaControl& meta) noexcept 
    : Super(impl::ctor_lock(parent), meta),
    m_oImage(this), m_oLabel(this) {
    // XXX: 硬编码
    m_oBox.margin = { 4, 2, 4, 2 };
    m_oBox.padding = { 4, 1, 2, 1 };
    m_state.focusable = true;
    // 原子性, 子控件为本控件的组成部分
    m_state.atomicity = true;
#ifdef LUI_ACCESSIBLE
    // 没有逻辑子控件
    m_pAccCtrl = nullptr;
#endif
    m_state.orient = Orient_Horizontal;
    m_oStyle.align = AttributeAlign::Align_Center;
    // 私有实现
    //UIControlPrivate::SetFocusable(image, false);
    //UIControlPrivate::SetFocusable(label, false);
#ifndef NDEBUG
    m_oImage.name_dbg = "checkbox::image";
    m_oLabel.name_dbg = "checkbox::label";
    assert(m_oLabel.IsFocusable() == false);
    assert(m_oImage.IsFocusable() == false);
    //label.SetText(u"复选框");
#endif
    // 设置连接控件
    m_oLabel.SetControl(*this);
    // 构造锁
    impl::ctor_unlock();
}


/// <summary>
/// Finalizes an instance of the <see cref="UICheckBox"/> class.
/// </summary>
/// <returns></returns>
LongUI::UICheckBox::~UICheckBox() noexcept {
    // 存在提前释放子控件, 需要标记"在析构中"
    m_state.destructing = true;
}



/// <summary>
/// Adds the attribute.
/// </summary>
/// <param name="key">The key.</param>
/// <param name="value">The value.</param>
/// <returns></returns>
void LongUI::UICheckBox::add_attribute(uint32_t key, U8View value) noexcept {
    // 新增属性列表
    constexpr auto BKDR_SRC         = 0x001E57C4_ui32;
    constexpr auto BKDR_VALUE       = 0x246df521_ui32;
    constexpr auto BKDR_ACCESSKEY   = 0xba56ab7b_ui32;
    // 分类讨论
    switch (key)
    {
    case "label"_bkdr:
        // 传递给子控件
        Unsafe::AddAttrUninited(m_oLabel, BKDR_VALUE, value);
        break;
    case BKDR_ACCESSKEY:
        // 传递给子控件
        Unsafe::AddAttrUninited(m_oLabel, key, value);
        break;
    case BKDR_SRC:
        // src: 使用图片
        this->SetImageSource(value);
        break;
    default:
        // 其他情况, 交给基类处理
        return Super::add_attribute(key, value);
    }
}

/// <summary>
/// Does the event.
/// </summary>
/// <param name="sender">The sender.</param>
/// <param name="arg">The argument.</param>
/// <returns></returns>
auto LongUI::UICheckBox::DoEvent(
    UIControl * sender, const EventArg & arg) noexcept -> EventAccept {
    // 分类讨论
    switch (arg.nevent)
    {
    case NoticeEvent::Event_DoAccessAction:
        // 默认行动
        this->SetAsDefaultAndFocus();
        this->Toggle();
        return Event_Accept;
    case NoticeEvent::Event_Initialize:
        // 初始化
        this->init_checkbox();
        [[fallthrough]];
    default:
        // 基类处理
        return Super::DoEvent(sender, arg);
    }
}

/// <summary>
/// Initializes the checkbox.
/// </summary>
/// <returns></returns>
void LongUI::UICheckBox::init_checkbox() noexcept {
    if (m_oStyle.appearance == Appearance_NotSet) {
        UIControlPrivate::SetAppearance(*this, Appearance_CheckBoxContainer);
        UIControlPrivate::SetAppearance(m_oImage, Appearance_CheckBox);
    }
    // 在attr中设置了checked状态?
    if (m_oStyle.state.checked) {
        UIControlPrivate::RefStyleState(m_oImage).checked = true;
    }
    // 在attr中设置了indeterminate状态?
    if (m_oStyle.state.indeterminate) {
        UIControlPrivate::RefStyleState(m_oImage).indeterminate = true;
    }
    // 同步image-disable状态
    if (m_oStyle.state.disabled) {
        UIControlPrivate::RefStyleState(m_oImage).disabled = true;
    }
}


/// <summary>
/// Does the mouse event.
/// </summary>
/// <param name="e">The e.</param>
/// <returns></returns>
auto LongUI::UICheckBox::DoMouseEvent(const MouseEventArg & e) noexcept -> EventAccept {
    // 左键弹起 修改状态
    switch (e.type)
    {
    case LongUI::MouseEvent::Event_LButtonUp:
        this->Toggle();
        [[fallthrough]];
    default:
        return Super::DoMouseEvent(e);
    }
}

/// <summary>
/// Gets the text.
/// </summary>
/// <returns></returns>
auto LongUI::UICheckBox::GetText() const noexcept -> const char16_t* {
    return m_oLabel.GetText();
}

/// <summary>
/// Gets the text string.
/// </summary>
/// <returns></returns>
auto LongUI::UICheckBox::GetTextString() const noexcept -> const CUIString& {
    return m_oLabel.GetTextString();
}

/// <summary>
/// Sets the text.
/// </summary>
/// <param name="text">The text.</param>
/// <returns></returns>
void LongUI::UICheckBox::SetText(CUIString&& text) noexcept {
    m_oLabel.SetText(std::move(text));
}

/// <summary>
/// Sets the text.
/// </summary>
/// <param name="text">The text.</param>
/// <returns></returns>
void LongUI::UICheckBox::SetText(const CUIString& text) noexcept {
    this->SetText(CUIString{ text });
}

/// <summary>
/// Sets the text.
/// </summary>
/// <param name="text">The text.</param>
/// <returns></returns>
void LongUI::UICheckBox::SetText(U16View text) noexcept {
    this->SetText(CUIString(text));
}

