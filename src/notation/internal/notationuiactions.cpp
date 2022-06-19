/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "notationuiactions.h"

#include <unordered_map>

#include "ui/view/iconcodes.h"

using namespace mu::notation;
using namespace mu::ui;
using namespace mu::actions;

static const ActionCode NOTE_INPUT_ACTION_CODE("note-input");

static const ActionCode SHOW_INVISIBLE_CODE("show-invisible");
static const ActionCode SHOW_UNPRINTABLE_CODE("show-unprintable");
static const ActionCode SHOW_FRAMES_CODE("show-frames");
static const ActionCode SHOW_PAGEBORDERS_CODE("show-pageborders");
static const ActionCode SHOW_IRREGULAR_CODE("show-irregular");

static const ActionCode TOGGLE_CONCERT_PITCH_CODE("concert-pitch");

//! NOTE Each notation actions should has context is UiCtxNotationOpened.
//! If you want what action to dispatch by shortcut only when notation is focused (ex notation-move-right by press Right key),
//! then you should set the shortcut context accordingly, not the action context.
//! Because actions can be dispatched not only shortcuts, but another way, ex by click Button, Menu and etc

const UiActionList NotationUiActions::m_actions = {
    UiAction("notation-escape",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Esc")
             ),
    UiAction("put-note", // args: PointF pos, bool replace, bool insert
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Put note")
             ),
    UiAction("remove-note", // args: PointF pos
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Remove note")
             ),
    UiAction("next-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next element"),
             QT_TRANSLATE_NOOP("action", "Accessibility: Next element")
             ),
    UiAction("prev-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous element"),
             QT_TRANSLATE_NOOP("action", "Accessibility: Previous element")
             ),
    UiAction("notation-move-right",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next chord / Shift text right"),
             QT_TRANSLATE_NOOP("action", "Go to next chord or shift text right")
             ),
    UiAction("notation-move-left",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous chord / Shift text left"),
             QT_TRANSLATE_NOOP("action", "Go to previous chord or shift text left")
             ),
    UiAction("notation-move-right-quickly",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next measure / Shift text right quickly"),
             QT_TRANSLATE_NOOP("action", "Go to next measure or shift text right quickly")
             ),
    UiAction("notation-move-left-quickly",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous measure / Shift text left quickly"),
             QT_TRANSLATE_NOOP("action", "Go to previous measure or shift text left quickly")
             ),
    UiAction("up-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Up note in chord"),
             QT_TRANSLATE_NOOP("action", "Go to higher pitched note in chord")
             ),
    UiAction("down-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Down note in chord"),
             QT_TRANSLATE_NOOP("action", "Go to lower pitched note in chord")
             ),
    UiAction("top-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Top note in chord"),
             QT_TRANSLATE_NOOP("action", "Go to top note in chord")
             ),
    UiAction("bottom-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Bottom note in chord"),
             QT_TRANSLATE_NOOP("action", "Go bottom note in chord")
             ),
    UiAction("first-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "First element"),
             QT_TRANSLATE_NOOP("action", "Go to first element in score")
             ),
    UiAction("last-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Last element"),
             QT_TRANSLATE_NOOP("action", "Go to last element in score")
             ),
    UiAction("move-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move to staff above"),
             QT_TRANSLATE_NOOP("action", "Move chord/rest to staff above")
             ),
    UiAction("move-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move to staff below"),
             QT_TRANSLATE_NOOP("action", "Move chord/rest to staff below")
             ),
    UiAction("next-track",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next staff or voice")
             ),
    UiAction("prev-track",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous staff or voice")
             ),
    UiAction("next-frame",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next frame")
             ),
    UiAction("prev-frame",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous frame")
             ),
    UiAction("next-system",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next system")
             ),
    UiAction("prev-system",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous system")
             ),
    UiAction("show-irregular",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Mark irregular measures")
             ),
    UiAction("toggle-insert-mode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'insert mode'")
             ),
    UiAction("select-next-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add next chord to selection")
             ),
    UiAction("select-prev-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add previous chord to selection")
             ),
    UiAction("move-left",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move chord/rest left")
             ),
    UiAction("move-right",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move chord/rest right")
             ),
    UiAction("pitch-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Up"),
             QT_TRANSLATE_NOOP("action", "Pitch up or move text or articulation up")
             ),
    UiAction("pitch-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Down"),
             QT_TRANSLATE_NOOP("action", "Pitch down or move text or articulation down")
             ),
    UiAction("pitch-down-octave",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Down octave"),
             QT_TRANSLATE_NOOP("action", "Pitch down by an octave or move text or articulation down")
             ),
    UiAction("pitch-up-octave",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Up octave"),
             QT_TRANSLATE_NOOP("action", "Pitch up by an octave or move text or articulation up")
             ),
    UiAction("double-duration",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double duration")
             ),
    UiAction("half-duration",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Halve duration")
             ),
    UiAction("inc-duration-dotted",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double selected duration (dotted)")
             ),
    UiAction("dec-duration-dotted",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Halve selected duration (dotted)")
             ),
    UiAction("notation-cut",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Cut"),
             IconCode::Code::CUT
             ),
    UiAction("notation-copy",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Copy"),
             IconCode::Code::COPY
             ),
    UiAction("notation-paste",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste"),
             IconCode::Code::PASTE
             ),
    UiAction("notation-paste-half",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste half duration")
             ),
    UiAction("notation-paste-double",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste double duration")
             ),
    UiAction("notation-paste-special",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste special")
             ),
    UiAction("notation-swap",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Swap with clipboard")
             ),
    UiAction("toggle-visible",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle visibility of elements")
             ),
    UiAction("notation-select-all",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select all")
             ),
    UiAction("notation-select-section",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select section")
             ),
    UiAction("select-similar",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Similar"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements")
             ),
    UiAction("select-similar-staff",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Similar on this staff"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements on the same staff")
             ),
    UiAction("select-similar-range",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Similar in this range"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements in the range selection")
             ),
    UiAction("select-dialog",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "More…"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements with more options")
             ),
    UiAction("notation-delete",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Delete"),
             QT_TRANSLATE_NOOP("action", "Delete the selected element(s)"),
             IconCode::Code::DELETE_TANK
             ),
    UiAction("edit-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Style…"),
             QT_TRANSLATE_NOOP("action", "Edit style")
             ),
    UiAction("page-settings",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page settings…"),
             QT_TRANSLATE_NOOP("action", "Page settings")
             ),
    UiAction("load-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Load style…"),
             QT_TRANSLATE_NOOP("action", "Load style")
             ),
    UiAction("save-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Save style…"),
             QT_TRANSLATE_NOOP("action", "Save style")
             ),
    UiAction("transpose",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "&Transpose…"),
             QT_TRANSLATE_NOOP("action", "Transpose")
             ),
    UiAction("explode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Explode"),
             QT_TRANSLATE_NOOP("action", "Explode contents of top selected staff into staves below")
             ),
    UiAction("implode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Implode"),
             QT_TRANSLATE_NOOP("action", "Implode contents of selected staves into top selected staff")
             ),
    UiAction("realize-chord-symbols",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Realize chord symbols"),
             QT_TRANSLATE_NOOP("action", "Convert chord symbols into notes")
             ),
    UiAction("time-delete",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Remove selected range"),
             IconCode::Code::DELETE_TANK
             ),
    UiAction("slash-fill",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fill with slashes")
             ),
    UiAction("slash-rhythm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'rhythmic slash notation'")
             ),
    UiAction("pitch-spell",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Respell pitches")
             ),
    UiAction("reset-groupings",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Regroup rhythms"),
             QT_TRANSLATE_NOOP("action", "Combine rests and tied notes from selection and resplit at rhythmical boundaries")
             ),
    UiAction("resequence-rehearsal-marks",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Resequence rehearsal marks")
             ),
    UiAction("unroll-repeats",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Unroll repeats")
             ),
    UiAction("copy-lyrics-to-clipboard",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Copy lyrics to clipboard")
             ),
    UiAction("del-empty-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Remove empty trailing measures")
             ),
    UiAction("parts",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Parts"),
             QT_TRANSLATE_NOOP("action", "Manage parts"),
             IconCode::Code::PAGE
             ),
    UiAction("view-mode-page",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page view"),
             IconCode::Code::PAGE_VIEW
             ),
    UiAction("view-mode-continuous",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Continuous view (horizontal)"),
             IconCode::Code::CONTINUOUS_VIEW
             ),
    UiAction("view-mode-single",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Continuous view (vertical)"),
             IconCode::Code::CONTINUOUS_VIEW_VERTICAL
             ),
    UiAction("find",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Find / Go To")
             ),
    UiAction("staff-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Staff/part properties…"),
             QT_TRANSLATE_NOOP("action", "Staff/part properties")
             ),
    UiAction("staff-text-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Staff text properties…"),
             QT_TRANSLATE_NOOP("action", "Staff text properties")
             ),
    UiAction("system-text-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "System text properties…"),
             QT_TRANSLATE_NOOP("action", "System text properties")
             ),
    UiAction("measure-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Measure properties…"),
             QT_TRANSLATE_NOOP("action", "Measure properties")
             ),
    UiAction("add-remove-breaks",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add/remove system breaks…"),
             QT_TRANSLATE_NOOP("action", "Add/remove system breaks")
             ),
    UiAction("undo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Undo"),
             QT_TRANSLATE_NOOP("action", "Undo last change"),
             IconCode::Code::UNDO
             ),
    UiAction("redo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Redo"),
             QT_TRANSLATE_NOOP("action", "Redo last undo"),
             IconCode::Code::REDO
             ),
    UiAction("voice-x12",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-2"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-2")
             ),
    UiAction("voice-x13",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-3"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-3")
             ),
    UiAction("voice-x14",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-4"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-4")
             ),
    UiAction("voice-x23",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-3"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-3")
             ),
    UiAction("voice-x24",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-4"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-4")
             ),
    UiAction("voice-x34",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 3-4"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 3-4")
             ),
    UiAction("system-break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle system break"),
             QT_TRANSLATE_NOOP("action", "Toggle 'system break'")
             ),
    UiAction("page-break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle page break"),
             QT_TRANSLATE_NOOP("action", "Toggle 'page break'")
             ),
/*
    UiAction("plui_system_break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 1")
             ),

    UiAction("plui_page_break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 2")
             ),

    UiAction("plui_section_break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 3")
             ),

    UiAction("plui_group_measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 4")
             ),

    UiAction("plui_staff_spacer_down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 5")
             ),

    UiAction("plui_staff_spacer_up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 6")
             ),

    UiAction("plui_staff_spacer_fixed_down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 7")
             ),

    UiAction("plui_staff_type_change",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 8")
             ),

    UiAction("plui_insert_vertical_frame",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 9")
             ),

    UiAction("plui_insert_horizontal_frame",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 10")
             ),

    UiAction("plui_insert_text_frame",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 11")
             ),

    UiAction("plui_insert_one_measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 12")
             ),

    UiAction("plui_single_grace_low_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 13")
             ),

    UiAction("plui_single_grace_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 14")
             ),

    UiAction("plui_single_grace_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 15")
             ),

    UiAction("plui_single_grace_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 16")
             ),

    UiAction("plui_single_grace_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 17")
             ),

    UiAction("plui_single_grace_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 18")
             ),

    UiAction("plui_single_grace_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 19")
             ),

    UiAction("plui_single_grace_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 20")
             ),

    UiAction("plui_single_grace_high_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 21")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 22")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 23")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 24")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 25")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 26")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 27")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 28")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 29")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 30")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 31")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 32")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 33")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 34")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 35")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 36")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 37")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 38")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 39")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 40")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 41")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 42")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 43")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 44")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 45")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 46")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 47")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 48")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 49")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 50")
             ),

    UiAction("plui_double_grace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 51")
             ),

    UiAction("plui_half_doubling_on_low_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 52")
             ),

    UiAction("plui_half_doubling_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 53")
             ),

    UiAction("plui_half_doubling_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 54")
             ),

    UiAction("plui_half_doubling_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 55")
             ),

    UiAction("plui_half_doubling_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 56")
             ),

    UiAction("plui_half_doubling_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 57")
             ),

    UiAction("plui_half_doubling_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 58")
             ),

    UiAction("plui_doubling_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 59")
             ),

    UiAction("plui_doubling_on_high_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 60")
             ),

    UiAction("plui_half_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 61")
             ),

    UiAction("plui_half_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 62")
             ),

    UiAction("plui_half_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 63")
             ),

    UiAction("plui_half_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 64")
             ),

    UiAction("plui_half_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 65")
             ),

    UiAction("plui_half_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 66")
             ),

    UiAction("plui_half_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 67")
             ),

    UiAction("plui_half_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 68")
             ),

    UiAction("plui_grip",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 69")
             ),

    UiAction("plui_half_d_throw",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 70")
             ),

    UiAction("plui_doubling_on_low_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 71")
             ),

    UiAction("plui_doubling_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 72")
             ),

    UiAction("plui_doubling_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 73")
             ),

    UiAction("plui_doubling_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 74")
             ),

    UiAction("plui_doubling_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 75")
             ),

    UiAction("plui_doubling_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 76")
             ),

    UiAction("plui_doubling_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 77")
             ),

    UiAction("plui_thumb_doubling_on_low_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 78")
             ),

    UiAction("plui_thumb_doubling_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 79")
             ),

    UiAction("plui_thumb_doubling_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 80")
             ),

    UiAction("plui_thumb_doubling_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 81")
             ),

    UiAction("plui_thumb_doubling_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 82")
             ),

    UiAction("plui_thumb_doubling_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 83")
             ),

    UiAction("plui_thumb_doubling_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 84")
             ),

    UiAction("plui_g_grace_note_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 85")
             ),

    UiAction("plui_g_grace_note_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 86")
             ),

    UiAction("plui_g_grace_note_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 87")
             ),

    UiAction("plui_g_grace_note_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 88")
             ),

    UiAction("plui_g_grace_note_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 89")
             ),

    UiAction("plui_g_grace_note_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 90")
             ),

    UiAction("plui_g_grace_note_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 91")
             ),

    UiAction("plui_double_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 92")
             ),

    UiAction("plui_double_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 93")
             ),

    UiAction("plui_double_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 94")
             ),

    UiAction("plui_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 95")
             ),

    UiAction("plui_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 96")
             ),

    UiAction("plui_double_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 97")
             ),

    UiAction("plui_double_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 98")
             ),

    UiAction("plui_double_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 99")
             ),

    UiAction("plui_double_strike_on_high_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 100")
             ),

    UiAction("plui_thumb_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 101")
             ),

    UiAction("plui_thumb_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 102")
             ),

    UiAction("plui_thumb_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 103")
             ),

    UiAction("plui_thumb_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 104")
             ),

    UiAction("plui_thumb_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 105")
             ),

    UiAction("plui_thumb_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 106")
             ),

    UiAction("plui_thumb_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 107")
             ),

    UiAction("plui_thumb_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 108")
             ),

    UiAction("plui_grip",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 109")
             ),

    UiAction("plui_grip",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 110")
             ),

    UiAction("plui_birl",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 111")
             ),

    UiAction("plui_d_throw",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 112")
             ),

    UiAction("plui_half_heavy_d_throw",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 113")
             ),

    UiAction("plui_taorluath",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 114")
             ),

    UiAction("plui_birl",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 115")
             ),

    UiAction("plui_bubbly",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 116")
             ),

    UiAction("plui_heavy_d_throw",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 117")
             ),

    UiAction("plui_taorluath",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 118")
             ),

    UiAction("plui_taorluath",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 119")
             ),

    UiAction("plui_half_double_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 120")
             ),

    UiAction("plui_half_double_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 121")
             ),

    UiAction("plui_half_double_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 122")
             ),

    UiAction("plui_half_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 123")
             ),

    UiAction("plui_half_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 124")
             ),

    UiAction("plui_half_double_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 125")
             ),

    UiAction("plui_half_double_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 126")
             ),

    UiAction("plui_half_double_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 127")
             ),

    UiAction("plui_half_double_strike_on_high_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 128")
             ),

    UiAction("plui_half_grip_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 129")
             ),

    UiAction("plui_half_grip_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 130")
             ),

    UiAction("plui_half_grip_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 131")
             ),

    UiAction("plui_half_grip_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 132")
             ),

    UiAction("plui_half_grip_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 133")
             ),

    UiAction("plui_half_grip_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 134")
             ),

    UiAction("plui_half_grip_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 135")
             ),

    UiAction("plui_half_grip_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 136")
             ),

    UiAction("plui_half_grip_on_high_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 137")
             ),

    UiAction("plui_half_pele_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 138")
             ),

    UiAction("plui_half_pele_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 139")
             ),

    UiAction("plui_half_pele_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 140")
             ),

    UiAction("plui_half_pele_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 141")
             ),

    UiAction("plui_half_pele_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 142")
             ),

    UiAction("plui_half_pele_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 143")
             ),

    UiAction("plui_half_pele_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 144")
             ),

    UiAction("plui_half_pele_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 145")
             ),

    UiAction("plui_g_grace_note_grip_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 146")
             ),

    UiAction("plui_g_grace_note_grip_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 147")
             ),

    UiAction("plui_g_grace_note_grip_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 148")
             ),

    UiAction("plui_g_grace_note_grip_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 149")
             ),

    UiAction("plui_g_grace_note_grip_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 150")
             ),

    UiAction("plui_g_grace_note_grip_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 151")
             ),

    UiAction("plui_g_grace_note_grip_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 152")
             ),

    UiAction("plui_thumb_grip_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 153")
             ),

    UiAction("plui_thumb_grip_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 154")
             ),

    UiAction("plui_thumb_grip_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 155")
             ),

    UiAction("plui_thumb_grip_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 156")
             ),

    UiAction("plui_thumb_grip_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 157")
             ),

    UiAction("plui_thumb_grip_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 158")
             ),

    UiAction("plui_thumb_grip_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 159")
             ),

    UiAction("plui_thumb_grip_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 160")
             ),

    UiAction("plui_bubbly",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 161")
             ),

    UiAction("plui_birl",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 162")
             ),

    UiAction("plui_birl",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 163")
             ),

    UiAction("plui_pele_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 164")
             ),

    UiAction("plui_pele_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 165")
             ),

    UiAction("plui_pele_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 166")
             ),

    UiAction("plui_pele_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 167")
             ),

    UiAction("plui_pele_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 168")
             ),

    UiAction("plui_pele_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 169")
             ),

    UiAction("plui_pele_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 170")
             ),

    UiAction("plui_thumb_grace_note_pele_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 171")
             ),

    UiAction("plui_thumb_grace_note_pele_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 172")
             ),

    UiAction("plui_thumb_grace_note_pele_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 173")
             ),

    UiAction("plui_thumb_grace_note_pele_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 174")
             ),

    UiAction("plui_thumb_grace_note_pele_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 175")
             ),

    UiAction("plui_thumb_grace_note_pele_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 176")
             ),

    UiAction("plui_thumb_grace_note_pele_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 177")
             ),

    UiAction("plui_thumb_grace_note_pele_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 178")
             ),

    UiAction("plui_g_grace_note_double_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 179")
             ),

    UiAction("plui_g_grace_note_double_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 180")
             ),

    UiAction("plui_g_grace_note_double_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 181")
             ),

    UiAction("plui_g_grace_note_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 182")
             ),

    UiAction("plui_g_grace_note_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 183")
             ),

    UiAction("plui_g_grace_note_double_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 184")
             ),

    UiAction("plui_g_grace_note_double_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 185")
             ),

    UiAction("plui_thumb_double_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 186")
             ),

    UiAction("plui_thumb_double_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 187")
             ),

    UiAction("plui_thumb_double_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 188")
             ),

    UiAction("plui_thumb_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 189")
             ),

    UiAction("plui_thumb_double_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 190")
             ),

    UiAction("plui_thumb_double_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 191")
             ),

    UiAction("plui_thumb_double_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 192")
             ),

    UiAction("plui_thumb_double_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 193")
             ),

    UiAction("plui_triple_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 194")
             ),

    UiAction("plui_triple_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 195")
             ),

    UiAction("plui_triple_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 196")
             ),

    UiAction("plui_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 197")
             ),

    UiAction("plui_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 198")
             ),

    UiAction("plui_triple_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 199")
             ),

    UiAction("plui_triple_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 200")
             ),

    UiAction("plui_triple_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 201")
             ),

    UiAction("plui_triple_strike_on_high_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 202")
             ),

    UiAction("plui_half_triple_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 203")
             ),

    UiAction("plui_half_triple_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 204")
             ),

    UiAction("plui_half_triple_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 205")
             ),

    UiAction("plui_half_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 206")
             ),

    UiAction("plui_half_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 207")
             ),

    UiAction("plui_half_triple_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 208")
             ),

    UiAction("plui_half_triple_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 209")
             ),

    UiAction("plui_half_triple_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 210")
             ),

    UiAction("plui_half_triple_strike_on_high_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 211")
             ),

    UiAction("plui_g_grace_note_triple_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 212")
             ),

    UiAction("plui_g_grace_note_triple_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 213")
             ),

    UiAction("plui_g_grace_note_triple_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 214")
             ),

    UiAction("plui_g_grace_note_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 215")
             ),

    UiAction("plui_g_grace_note_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 216")
             ),

    UiAction("plui_g_grace_note_triple_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 217")
             ),

    UiAction("plui_g_grace_note_triple_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 218")
             ),

    UiAction("plui_thumb_triple_strike_on_low_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 219")
             ),

    UiAction("plui_thumb_triple_strike_on_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 220")
             ),

    UiAction("plui_thumb_triple_strike_on_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 221")
             ),

    UiAction("plui_thumb_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 222")
             ),

    UiAction("plui_thumb_triple_strike_on_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 223")
             ),

    UiAction("plui_thumb_triple_strike_on_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 224")
             ),

    UiAction("plui_thumb_triple_strike_on_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 225")
             ),

    UiAction("plui_thumb_triple_strike_on_high_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 226")
             ),

    UiAction("plui_combining_accordion_coupler_dot",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 227")
             ),

    UiAction("plui_combining_left_hand,_2_ranks,_empty",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 228")
             ),

    UiAction("plui_combining_left_hand,_3_ranks,_empty_(square)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 229")
             ),

    UiAction("plui_combining_right_hand,_3_ranks,_empty",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 230")
             ),

    UiAction("plui_combining_right_hand,_4_ranks,_empty",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 231")
             ),

    UiAction("plui_diatonic_accordion_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 232")
             ),

    UiAction("plui_left_hand,_2_ranks,_16__stop_(round)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 233")
             ),

    UiAction("plui_left_hand,_2_ranks,_8__stop_+_16__stop_(round)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 234")
             ),

    UiAction("plui_left_hand,_2_ranks,_8__stop_(round)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 235")
             ),

    UiAction("plui_left_hand,_2_ranks,_full_master_(round)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 236")
             ),

    UiAction("plui_left_hand,_2_ranks,_master_+_16__stop_(round)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 237")
             ),

    UiAction("plui_left_hand,_2_ranks,_master_(round)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 238")
             ),

    UiAction("plui_left_hand,_3_ranks,_2__stop_+_8__stop_(square)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 239")
             ),

    UiAction("plui_left_hand,_3_ranks,_2__stop_(square)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 240")
             ),

    UiAction("plui_left_hand,_3_ranks,_8__stop_(square)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 241")
             ),

    UiAction("plui_left_hand,_3_ranks,_double_8__stop_(square)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 242")
             ),

    UiAction("plui_left_hand,_3_ranks,_2__stop_+_double_8__stop_(tutti)_(square)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 243")
             ),

    UiAction("plui_pull",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 244")
             ),

    UiAction("plui_push",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 245")
             ),

    UiAction("plui_right_hand,_3_ranks,_8__stop_+_upper_tremolo_8__stop_+_16__stop_(accordion)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 246")
             ),

    UiAction("plui_right_hand,_3_ranks,_lower_tremolo_8__stop_+_8__stop_+_upper_tremolo_8__stop_(authentic_musette)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 247")
             ),

    UiAction("plui_right_hand,_3_ranks,_8__stop_+_16__stop_(bandoneón)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 248")
             ),

    UiAction("plui_right_hand,_3_ranks,_16__stop_(bassoon)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 249")
             ),

    UiAction("plui_right_hand,_3_ranks,_8__stop_(clarinet)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 250")
             ),

    UiAction("plui_right_hand,_3_ranks,_lower_tremolo_8__stop_+_8__stop_+_upper_tremolo_8__stop_+_16__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 251")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_lower_tremolo_8__stop_+_8__stop_+_upper_tremolo_8__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 252")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_lower_tremolo_8__stop_+_8__stop_+_upper_tremolo_8__stop_+_16__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 253")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_8__stop_+_16__stop_(harmonium)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 254")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_8__stop_+_upper_tremolo_8__stop_(imitation_musette)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 255")
             ),

    UiAction("plui_right_hand,_3_ranks,_lower_tremolo_8__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 256")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_lower_tremolo_8__stop_+_upper_tremolo_8__stop_+_16__stop_(master)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 257")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_8__stop_(oboe)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 258")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_16__stop_(organ)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 259")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_(piccolo)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 260")
             ),

    UiAction("plui_right_hand,_3_ranks,_lower_tremolo_8__stop_+_upper_tremolo_8__stop_+_16__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 261")
             ),

    UiAction("plui_right_hand,_3_ranks,_4__stop_+_lower_tremolo_8__stop_+_upper_tremolo_8__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 262")
             ),

    UiAction("plui_right_hand,_3_ranks,_lower_tremolo_8__stop_+_upper_tremolo_8__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 263")
             ),

    UiAction("plui_right_hand,_3_ranks,_upper_tremolo_8__stop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 264")
             ),

    UiAction("plui_right_hand,_3_ranks,_8__stop_+_upper_tremolo_8__stop_(violin)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 265")
             ),

    UiAction("plui_right_hand,_4_ranks,_alto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 266")
             ),

    UiAction("plui_right_hand,_4_ranks,_bass/alto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 267")
             ),

    UiAction("plui_right_hand,_4_ranks,_master",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 268")
             ),

    UiAction("plui_right_hand,_4_ranks,_soft_bass",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 269")
             ),

    UiAction("plui_right_hand,_4_ranks,_soft_tenor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 270")
             ),

    UiAction("plui_right_hand,_4_ranks,_soprano",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 271")
             ),

    UiAction("plui_right_hand,_4_ranks,_tenor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 272")
             ),

    UiAction("plui_ricochet_(2_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 273")
             ),

    UiAction("plui_ricochet_(3_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 274")
             ),

    UiAction("plui_ricochet_(4_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 275")
             ),

    UiAction("plui_ricochet_(5_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 276")
             ),

    UiAction("plui_ricochet_(6_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 277")
             ),

    UiAction("plui_combining_ricochet_for_stem_(2_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 278")
             ),

    UiAction("plui_combining_ricochet_for_stem_(3_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 279")
             ),

    UiAction("plui_combining_ricochet_for_stem_(4_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 280")
             ),

    UiAction("plui_combining_ricochet_for_stem_(5_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 281")
             ),

    UiAction("plui_combining_ricochet_for_stem_(6_tones)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 282")
             ),

    UiAction("plui_c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 283")
             ),

    UiAction("plui_cm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 284")
             ),

    UiAction("plui_c7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 285")
             ),

    UiAction("plui_d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 286")
             ),

    UiAction("plui_dm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 287")
             ),

    UiAction("plui_d7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 288")
             ),

    UiAction("plui_e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 289")
             ),

    UiAction("plui_em",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 290")
             ),

    UiAction("plui_e7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 291")
             ),

    UiAction("plui_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 292")
             ),

    UiAction("plui_fm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 293")
             ),

    UiAction("plui_f7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 294")
             ),

    UiAction("plui_g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 295")
             ),

    UiAction("plui_gm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 296")
             ),

    UiAction("plui_g7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 297")
             ),

    UiAction("plui_a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 298")
             ),

    UiAction("plui_am",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 299")
             ),

    UiAction("plui_a7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 300")
             ),

    UiAction("plui_b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 301")
             ),

    UiAction("plui_bm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 302")
             ),

    UiAction("plui_b7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 303")
             ),

    UiAction("plui_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 304")
             ),

    UiAction("plui_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 305")
             ),

    UiAction("plui_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 306")
             ),

    UiAction("plui_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 307")
             ),

    UiAction("plui_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 308")
             ),

    UiAction("plui_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 309")
             ),

    UiAction("plui_rh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 310")
             ),

    UiAction("plui_rh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 311")
             ),

    UiAction("plui_rh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 312")
             ),

    UiAction("plui_rh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 313")
             ),

    UiAction("plui_rh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 314")
             ),

    UiAction("plui_lh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 315")
             ),

    UiAction("plui_lh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 316")
             ),

    UiAction("plui_lh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 317")
             ),

    UiAction("plui_lh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 318")
             ),

    UiAction("plui_lh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 319")
             ),

    UiAction("plui_lh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 320")
             ),

    UiAction("plui_lh_guitar_fingering_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 321")
             ),

    UiAction("plui_string_number_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 322")
             ),

    UiAction("plui_string_number_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 323")
             ),

    UiAction("plui_string_number_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 324")
             ),

    UiAction("plui_string_number_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 325")
             ),

    UiAction("plui_string_number_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 326")
             ),

    UiAction("plui_string_number_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 327")
             ),

    UiAction("plui_string_number_%1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 328")
             ),

    UiAction("plui_thumb_position",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 329")
             ),

    UiAction("plui_right-hand_fingering,_thumb",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 330")
             ),

    UiAction("plui_right-hand_fingering,_first_finger",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 331")
             ),

    UiAction("plui_right-hand_fingering,_second_finger",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 332")
             ),

    UiAction("plui_right-hand_fingering,_third_finger",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 333")
             ),

    UiAction("plui_eighth_through_stem",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 334")
             ),

    UiAction("plui_16th_through_stem",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 335")
             ),

    UiAction("plui_32nd_through_stem",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 336")
             ),

    UiAction("plui_64th_through_stem",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 337")
             ),

    UiAction("plui_buzz_roll",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 338")
             ),

    UiAction("plui_eighth_between_notes",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 339")
             ),

    UiAction("plui_16th_between_notes",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 340")
             ),

    UiAction("plui_32nd_between_notes",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 341")
             ),

    UiAction("plui_64th_between_notes",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 342")
             ),

    UiAction("plui_arpeggio",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 343")
             ),

    UiAction("plui_up_arpeggio",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 344")
             ),

    UiAction("plui_down_arpeggio",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 345")
             ),

    UiAction("plui_bracket_arpeggio",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 346")
             ),

    UiAction("plui_up_arpeggio_straight",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 347")
             ),

    UiAction("plui_down_arpeggio_straight",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 348")
             ),

    UiAction("plui_straight_glissando",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 349")
             ),

    UiAction("plui_wavy_glissando",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 350")
             ),

    UiAction("plui_fall",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 351")
             ),

    UiAction("plui_doit",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 352")
             ),

    UiAction("plui_plop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 353")
             ),

    UiAction("plui_scoop",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 354")
             ),

    UiAction("plui_slide_out_down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 355")
             ),

    UiAction("plui_slide_out_up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 356")
             ),

    UiAction("plui_slide_in_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 357")
             ),

    UiAction("plui_slide_in_below",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 358")
             ),

    UiAction("plui_inverted_turn",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 359")
             ),

    UiAction("plui_turn_with_slash",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 360")
             ),

    UiAction("plui_turn",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 361")
             ),

    UiAction("plui_trill",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 362")
             ),

    UiAction("plui_short_trill",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 363")
             ),

    UiAction("plui_mordent",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 364")
             ),

    UiAction("plui_tremblement",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 365")
             ),

    UiAction("plui_prall_mordent",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 366")
             ),

    UiAction("plui_up_prall",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 367")
             ),

    UiAction("plui_mordent_with_upper_prefix",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 368")
             ),

    UiAction("plui_up_mordent",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 369")
             ),

    UiAction("plui_down_mordent",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 370")
             ),

    UiAction("plui_prall_down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 371")
             ),

    UiAction("plui_prall_up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 372")
             ),

    UiAction("plui_line_prall",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 373")
             ),

    UiAction("plui_slide",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 374")
             ),

    UiAction("plui_flat",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 375")
             ),

    UiAction("plui_natural",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 376")
             ),

    UiAction("plui_sharp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 377")
             ),

    UiAction("plui_double_sharp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 378")
             ),

    UiAction("plui_double_flat",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 379")
             ),

    UiAction("plui_triple_sharp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 380")
             ),

    UiAction("plui_triple_flat",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 381")
             ),

    UiAction("plui_natural_flat",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 382")
             ),

    UiAction("plui_natural_sharp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 383")
             ),

    UiAction("plui_add_parentheses_to_element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 384")
             ),

    UiAction("plui_add_brackets_to_accidental",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 385")
             ),

    UiAction("plui_acciaccatura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 386")
             ),

    UiAction("plui_appoggiatura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 387")
             ),

    UiAction("plui_grace:_quarter",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 388")
             ),

    UiAction("plui_grace:_16th",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 389")
             ),

    UiAction("plui_grace:_32nd",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 390")
             ),

    UiAction("plui_grace:_8th_after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 391")
             ),

    UiAction("plui_grace:_16th_after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 392")
             ),

    UiAction("plui_grace:_32nd_after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 393")
             ),

    UiAction("plui_bracket",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 394")
             ),

    UiAction("plui_brace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 395")
             ),

    UiAction("plui_square",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 396")
             ),

    UiAction("plui_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 397")
             ),

    UiAction("plui_fermata_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 398")
             ),

    UiAction("plui_short_fermata_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 399")
             ),

    UiAction("plui_long_fermata_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 400")
             ),

    UiAction("plui_long_fermata_(henze)_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 401")
             ),

    UiAction("plui_short_fermata_(henze)_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 402")
             ),

    UiAction("plui_very_long_fermata_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 403")
             ),

    UiAction("plui_very_short_fermata_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 404")
             ),

    UiAction("plui_breath_mark_(comma)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 405")
             ),

    UiAction("plui_breath_mark_(tick-like)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 406")
             ),

    UiAction("plui_breath_mark_(salzedo)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 407")
             ),

    UiAction("plui_breath_mark_(upbow-like)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 408")
             ),

    UiAction("plui_curved_caesura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 409")
             ),

    UiAction("plui_caesura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 410")
             ),

    UiAction("plui_short_caesura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 411")
             ),

    UiAction("plui_thick_caesura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 412")
             ),

    UiAction("plui_caesura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 413")
             ),

    UiAction("plui_treble_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 414")
             ),

    UiAction("plui_treble_clef_8va_alta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 415")
             ),

    UiAction("plui_treble_clef_15ma_alta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 416")
             ),

    UiAction("plui_treble_clef_8va_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 417")
             ),

    UiAction("plui_treble_clef_15ma_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 418")
             ),

    UiAction("plui_double_treble_clef_8va_bassa_on_2nd_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 419")
             ),

    UiAction("plui_treble_clef_optional_8va_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 420")
             ),

    UiAction("plui_french_violin_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 421")
             ),

    UiAction("plui_soprano_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 422")
             ),

    UiAction("plui_mezzo-soprano_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 423")
             ),

    UiAction("plui_alto_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 424")
             ),

    UiAction("plui_tenor_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 425")
             ),

    UiAction("plui_baritone_clef_(c_clef)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 426")
             ),

    UiAction("plui_bass_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 427")
             ),

    UiAction("plui_bass_clef_8va_alta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 428")
             ),

    UiAction("plui_bass_clef_15ma_alta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 429")
             ),

    UiAction("plui_bass_clef_8va_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 430")
             ),

    UiAction("plui_bass_clef_15ma_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 431")
             ),

    UiAction("plui_baritone_clef_(f_clef)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 432")
             ),

    UiAction("plui_subbass_clef",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 433")
             ),

    UiAction("plui_percussion",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 434")
             ),

    UiAction("plui_percussion_2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 435")
             ),

    UiAction("plui_tablature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 436")
             ),

    UiAction("plui_tablature_4_lines",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 437")
             ),

    UiAction("plui_g_major,_e_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 438")
             ),

    UiAction("plui_d_major,_b_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 439")
             ),

    UiAction("plui_a_major,_f♯_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 440")
             ),

    UiAction("plui_e_major,_c♯_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 441")
             ),

    UiAction("plui_b_major,_g♯_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 442")
             ),

    UiAction("plui_f♯_major,_d♯_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 443")
             ),

    UiAction("plui_c♯_major,_a♯_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 444")
             ),

    UiAction("plui_c♭_major,_a♭_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 445")
             ),

    UiAction("plui_g♭_major,_e♭_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 446")
             ),

    UiAction("plui_d♭_major,_b♭_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 447")
             ),

    UiAction("plui_a♭_major,_f_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 448")
             ),

    UiAction("plui_e♭_major,_c_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 449")
             ),

    UiAction("plui_b♭_major,_g_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 450")
             ),

    UiAction("plui_f_major,_d_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 451")
             ),

    UiAction("plui_c_major,_a_minor",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 452")
             ),

    UiAction("plui_open/atonal",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 453")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 454")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 455")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 456")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 457")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 458")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 459")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 460")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 461")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 462")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 463")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 464")
             ),

    UiAction("plui_key_signature",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 465")
             ),

    UiAction("plui_2/4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 466")
             ),

    UiAction("plui_3/4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 467")
             ),

    UiAction("plui_4/4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 468")
             ),

    UiAction("plui_5/4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 469")
             ),

    UiAction("plui_6/4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 470")
             ),

    UiAction("plui_3/8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 471")
             ),

    UiAction("plui_6/8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 472")
             ),

    UiAction("plui_7/8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 473")
             ),

    UiAction("plui_9/8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 474")
             ),

    UiAction("plui_12/8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 475")
             ),

    UiAction("plui_common_time",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 476")
             ),

    UiAction("plui_cut_time",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 477")
             ),

    UiAction("plui_2/2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 478")
             ),

    UiAction("plui_3/2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 479")
             ),

    UiAction("plui_4/2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 480")
             ),

    UiAction("plui_cut_time_(bach)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 481")
             ),

    UiAction("plui_cut_triple_time_(9/8)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 482")
             ),

    UiAction("plui_accent_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 483")
             ),

    UiAction("plui_staccato_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 484")
             ),

    UiAction("plui_staccatissimo_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 485")
             ),

    UiAction("plui_tenuto_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 486")
             ),

    UiAction("plui_louré_(tenuto-staccato)_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 487")
             ),

    UiAction("plui_marcato_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 488")
             ),

    UiAction("plui_accent-staccato_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 489")
             ),

    UiAction("plui_laissez_vibrer_(l.v.)_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 490")
             ),

    UiAction("plui_marcato-staccato_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 491")
             ),

    UiAction("plui_marcato-tenuto_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 492")
             ),

    UiAction("plui_staccatissimo_stroke_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 493")
             ),

    UiAction("plui_staccatissimo_wedge_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 494")
             ),

    UiAction("plui_stress_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 495")
             ),

    UiAction("plui_tenuto-accent_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 496")
             ),

    UiAction("plui_unstress_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 497")
             ),

    UiAction("plui_soft_accent_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 498")
             ),

    UiAction("plui_soft_accent-staccato_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 499")
             ),

    UiAction("plui_soft_accent-tenuto_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 500")
             ),

    UiAction("plui_soft_accent-tenuto-staccato_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 501")
             ),

    UiAction("plui_fade_in",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 502")
             ),

    UiAction("plui_fade_out",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 503")
             ),

    UiAction("plui_volume_swell",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 504")
             ),

    UiAction("plui_sawtooth_line_segment",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 505")
             ),

    UiAction("plui_wide_sawtooth_line_segment",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 506")
             ),

    UiAction("plui_vibrato_large,_faster",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 507")
             ),

    UiAction("plui_vibrato_large,_slowest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 508")
             ),

    UiAction("plui_open",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 509")
             ),

    UiAction("plui_muted_(closed)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 510")
             ),

    UiAction("plui_harmonic",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 511")
             ),

    UiAction("plui_up_bow",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 512")
             ),

    UiAction("plui_down_bow",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 513")
             ),

    UiAction("plui_snap_pizzicato_above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 514")
             ),

    UiAction("plui_bend",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 515")
             ),

    UiAction("plui_tremolo_bar",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 516")
             ),

    UiAction("plui_normal",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 517")
             ),

    UiAction("plui_cross",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 518")
             ),

    UiAction("plui_plus",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 519")
             ),

    UiAction("plui_xcircle",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 520")
             ),

    UiAction("plui_with_x",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 521")
             ),

    UiAction("plui_triangle_up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 522")
             ),

    UiAction("plui_triangle_down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 523")
             ),

    UiAction("plui_slashed_(forwards)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 524")
             ),

    UiAction("plui_slashed_(backwards)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 525")
             ),

    UiAction("plui_diamond",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 526")
             ),

    UiAction("plui_diamond_(old)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 527")
             ),

    UiAction("plui_circled",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 528")
             ),

    UiAction("plui_circled_large",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 529")
             ),

    UiAction("plui_large_arrow",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 530")
             ),

    UiAction("plui_alt._brevis",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 531")
             ),

    UiAction("plui_slash",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 532")
             ),

    UiAction("plui_sol",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 533")
             ),

    UiAction("plui_la",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 534")
             ),

    UiAction("plui_fa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 535")
             ),

    UiAction("plui_mi",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 536")
             ),

    UiAction("plui_do",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 537")
             ),

    UiAction("plui_re",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 538")
             ),

    UiAction("plui_ti",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 539")
             ),

    UiAction("plui_add_parentheses_to_element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 540")
             ),

    UiAction("plui_slur",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 541")
             ),

    UiAction("plui_crescendo_hairpin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 542")
             ),

    UiAction("plui_diminuendo_hairpin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 543")
             ),

    UiAction("plui_crescendo_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 544")
             ),

    UiAction("plui_diminuendo_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 545")
             ),

    UiAction("plui_dynamic_+_hairpin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 546")
             ),

    UiAction("plui_prima_volta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 547")
             ),

    UiAction("plui_seconda_volta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 548")
             ),

    UiAction("plui_terza_volta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 549")
             ),

    UiAction("plui_seconda_volta,_open",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 550")
             ),

    UiAction("plui_8va_alta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 551")
             ),

    UiAction("plui_8va_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 552")
             ),

    UiAction("plui_15ma_alta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 553")
             ),

    UiAction("plui_15ma_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 554")
             ),

    UiAction("plui_22ma_alta",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 555")
             ),

    UiAction("plui_22ma_bassa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 556")
             ),

    UiAction("plui_pedal_(with_ped_and_line)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 557")
             ),

    UiAction("plui_pedal_(with_ped_and_asterisk)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 558")
             ),

    UiAction("plui_pedal_(straight_hooks)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 559")
             ),

    UiAction("plui_pedal_(angled_end_hook)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 560")
             ),

    UiAction("plui_pedal_(both_hooks_angled)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 561")
             ),

    UiAction("plui_pedal_(angled_start_hook)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 562")
             ),

    UiAction("plui_trill_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 563")
             ),

    UiAction("plui_upprall_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 564")
             ),

    UiAction("plui_downprall_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 565")
             ),

    UiAction("plui_prallprall_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 566")
             ),

    UiAction("plui_text_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 567")
             ),

    UiAction("plui_line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 568")
             ),

    UiAction("plui_ambitus",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 569")
             ),

    UiAction("plui_let_ring",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 570")
             ),

    UiAction("plui_guitar_vibrato",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 571")
             ),

    UiAction("plui_guitar_vibrato_wide",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 572")
             ),

    UiAction("plui_vibrato_sawtooth",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 573")
             ),

    UiAction("plui_tremolo_sawtooth_wide",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 574")
             ),

    UiAction("plui_palm_mute",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 575")
             ),

    UiAction("plui_single_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 576")
             ),

    UiAction("plui_double_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 577")
             ),

    UiAction("plui_left_(start)_repeat_sign",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 578")
             ),

    UiAction("plui_right_(end)_repeat_sign",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 579")
             ),

    UiAction("plui_dashed_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 580")
             ),

    UiAction("plui_final_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 581")
             ),

    UiAction("plui_right_and_left_repeat_sign",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 582")
             ),

    UiAction("plui_dotted_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 583")
             ),

    UiAction("plui_reverse_final_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 584")
             ),

    UiAction("plui_heavy_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 585")
             ),

    UiAction("plui_heavy_double_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 586")
             ),

    UiAction("plui_dashed_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 587")
             ),

    UiAction("plui_tick_barline_2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 588")
             ),

    UiAction("plui_short_barline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 589")
             ),

    UiAction("plui_short_barline_2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 590")
             ),

    UiAction("plui_staff_text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 591")
             ),

    UiAction("plui_expression_text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 592")
             ),

    UiAction("plui_instrument_change",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 593")
             ),

    UiAction("plui_rehearsal_mark",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 594")
             ),

    UiAction("plui_straight",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 595")
             ),

    UiAction("plui_system_text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 596")
             ),

    UiAction("plui_measure_number",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 597")
             ),

    UiAction("plui_swing",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 598")
             ),

    UiAction("plui_half_note_=_80_bpm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 599")
             ),

    UiAction("plui_quarter_note_=_80_bpm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 600")
             ),

    UiAction("plui_eighth_note_=_80_bpm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 601")
             ),

    UiAction("plui_dotted_half_note_=_80_bpm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 602")
             ),

    UiAction("plui_dotted_quarter_note_=_80_bpm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 603")
             ),

    UiAction("plui_dotted_eighth_note_=_80_bpm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 604")
             ),

    UiAction("plui_grave",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 605")
             ),

    UiAction("plui_largo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 606")
             ),

    UiAction("plui_lento",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 607")
             ),

    UiAction("plui_larghetto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 608")
             ),

    UiAction("plui_adagio",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 609")
             ),

    UiAction("plui_andante",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 610")
             ),

    UiAction("plui_andantino",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 611")
             ),

    UiAction("plui_moderato",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 612")
             ),

    UiAction("plui_allegretto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 613")
             ),

    UiAction("plui_allegro_moderato",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 614")
             ),

    UiAction("plui_allegro",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 615")
             ),

    UiAction("plui_vivace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 616")
             ),

    UiAction("plui_presto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 617")
             ),

    UiAction("plui_prestissimo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 618")
             ),

    UiAction("plui_quarter_note_=_dotted_quarter_note_metric_modulation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 619")
             ),

    UiAction("plui_dotted_quarter_note_=_quarter_note_metric_modulation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 620")
             ),

    UiAction("plui_half_note_=_quarter_note_metric_modulation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 621")
             ),

    UiAction("plui_quarter_note_=_half_note_metric_modulation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 622")
             ),

    UiAction("plui_eighth_note_=_eighth_note_metric_modulation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 623")
             ),

    UiAction("plui_quarter_note_=_quarter_note_metric_modulation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 624")
             ),

    UiAction("plui_dotted_eighth_note_=_quarter_note_metric_modulation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 625")
             ),

    UiAction("plui_ppp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 626")
             ),

    UiAction("plui_pp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 627")
             ),

    UiAction("plui_p",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 628")
             ),

    UiAction("plui_mp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 629")
             ),

    UiAction("plui_mf",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 630")
             ),

    UiAction("plui_f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 631")
             ),

    UiAction("plui_ff",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 632")
             ),

    UiAction("plui_fff",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 633")
             ),

    UiAction("plui_fp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 634")
             ),

    UiAction("plui_pf",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 635")
             ),

    UiAction("plui_sf",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 636")
             ),

    UiAction("plui_sfz",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 637")
             ),

    UiAction("plui_sff",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 638")
             ),

    UiAction("plui_sffz",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 639")
             ),

    UiAction("plui_sfp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 640")
             ),

    UiAction("plui_sfpp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 641")
             ),

    UiAction("plui_rfz",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 642")
             ),

    UiAction("plui_rf",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 643")
             ),

    UiAction("plui_fz",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 644")
             ),

    UiAction("plui_m",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 645")
             ),

    UiAction("plui_r",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 646")
             ),

    UiAction("plui_s",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 647")
             ),

    UiAction("plui_z",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 648")
             ),

    UiAction("plui_n",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 649")
             ),

    UiAction("plui_repeat_last_bar",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 650")
             ),

    UiAction("plui_repeat_last_two_bars",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 651")
             ),

    UiAction("plui_repeat_last_four_bars",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 652")
             ),

    UiAction("plui_segno",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 653")
             ),

    UiAction("plui_segno_variation",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 654")
             ),

    UiAction("plui_coda",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 655")
             ),

    UiAction("plui_varied_coda",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 656")
             ),

    UiAction("plui_fine",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 657")
             ),

    UiAction("plui_to_coda",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 658")
             ),

    UiAction("plui_to_coda_(symbol)",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 659")
             ),

    UiAction("plui_da_capo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 660")
             ),

    UiAction("plui_da_capo_al_fine",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 661")
             ),

    UiAction("plui_da_capo_al_coda",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 662")
             ),

    UiAction("plui_d.s._al_coda",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 663")
             ),

    UiAction("plui_d.s._al_fine",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 664")
             ),

    UiAction("plui_d.s.",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 665")
             ),

    UiAction("plui_left_(start)_repeat_sign",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 666")
             ),

    UiAction("plui_right_(end)_repeat_sign",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 667")
             ),

    UiAction("plui_right_and_left_repeat_sign",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 668")
             ),

    UiAction("plui_beam_start",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 669")
             ),

    UiAction("plui_beam_middle",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 670")
             ),

    UiAction("plui_no_beam",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 671")
             ),

    UiAction("plui_auto_beam",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Test action 672")
             ),
*/
    UiAction("section-break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle section break"),
             QT_TRANSLATE_NOOP("action", "Toggle 'section break'")
             ),
    UiAction("split-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Split measure before selected note/rest")
             ),
    UiAction("join-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Join selected measures")
             ),
    UiAction("insert-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert one measure before selection"),
             IconCode::Code::INSERT_ONE_MEASURE
             ),
    UiAction("insert-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures before selection…")
             ),
    UiAction("insert-measures-after-selection",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures after selection…")
             ),
    UiAction("insert-measures-at-start-of-score",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures at start of score…")
             ),
    UiAction("append-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert one measure at end of score")
             ),
    UiAction("append-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures at end of score…")
             ),
    UiAction("insert-hbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert horizontal frame"),
             IconCode::Code::HORIZONTAL_FRAME
             ),
    UiAction("insert-vbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert vertical frame"),
             IconCode::Code::VERTICAL_FRAME
             ),
    UiAction("insert-textframe",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert text frame"),
             IconCode::Code::TEXT_FRAME
             ),
    UiAction("append-hbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Append horizontal frame")
             ),
    UiAction("append-vbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Append vertical frame")
             ),
    UiAction("append-textframe",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Append text frame")
             ),
    UiAction("acciaccatura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Acciaccatura"),
             IconCode::Code::ACCIACCATURA
             ),
    UiAction("appoggiatura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Appoggiatura"),
             IconCode::Code::APPOGGIATURA
             ),
    UiAction("grace4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: quarter"),
             IconCode::Code::GRACE4
             ),
    UiAction("grace16",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 16th"),
             IconCode::Code::GRACE16
             ),
    UiAction("grace32",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 32nd"),
             IconCode::Code::GRACE32
             ),
    UiAction("grace8after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 8th after"),
             IconCode::Code::GRACE8_AFTER
             ),
    UiAction("grace16after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 16th after"),
             IconCode::Code::GRACE16_AFTER
             ),
    UiAction("grace32after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 32nd after"),
             IconCode::Code::GRACE32_AFTER
             ),
    UiAction("beam-auto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Auto beam"),
             IconCode::Code::AUTO_TEXT
             ),
    UiAction("beam-none",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "No beam"),
             IconCode::Code::NOTE_HEAD_EIGHTH
             ),
    UiAction("beam-break-left",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Break beam left"),
             IconCode::Code::BEAM_BREAK_LEFT
             ),
    UiAction("beam-break-inner-8th",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Break inner beams (8th)"),
             IconCode::Code::BEAM_BREAK_INNER_8TH
             ),
    UiAction("beam-break-inner-16th",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Break inner beams (16th)"),
             IconCode::Code::BEAM_BREAK_INNER_16TH
             ),
    UiAction("beam-join",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Join beams"),
             IconCode::Code::BEAM_JOIN
             ),
    UiAction("beam-feathered-decelerate",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Feathered beam, decelerate"),
             IconCode::Code::BEAM_FEATHERED_DECELERATE
             ),
    UiAction("beam-feathered-accelerate",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Feathered beam, accelerate"),
             IconCode::Code::BEAM_FEATHERED_ACCELERATE
             ),
    UiAction("add-brackets",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add brackets to accidental"),
             IconCode::Code::BRACKET
             ),
    UiAction("add-braces",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add braces to element"),
             IconCode::Code::BRACE
             ),
    UiAction("add-parentheses",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add parentheses to element"),
             IconCode::Code::BRACKET_PARENTHESES
             ),
    UiAction("interval1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Unison above"),
             QT_TRANSLATE_NOOP("action", "Enter unison above")
             ),
    UiAction("interval2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Second above"),
             QT_TRANSLATE_NOOP("action", "Enter second above")
             ),
    UiAction("interval3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Third above"),
             QT_TRANSLATE_NOOP("action", "Enter third above")
             ),
    UiAction("interval4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fourth above"),
             QT_TRANSLATE_NOOP("action", "Enter fourth above")
             ),
    UiAction("interval5",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fifth above"),
             QT_TRANSLATE_NOOP("action", "Enter fifth above")
             ),
    UiAction("interval6",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sixth above"),
             QT_TRANSLATE_NOOP("action", "Enter sixth above")
             ),
    UiAction("interval7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Seventh above"),
             QT_TRANSLATE_NOOP("action", "Enter seventh above")
             ),
    UiAction("interval8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Octave above"),
             QT_TRANSLATE_NOOP("action", "Enter octave above")
             ),
    UiAction("interval9",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ninth above"),
             QT_TRANSLATE_NOOP("action", "Enter ninth above")
             ),
    UiAction("interval-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Second below"),
             QT_TRANSLATE_NOOP("action", "Enter second below")
             ),
    UiAction("interval-3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Third below"),
             QT_TRANSLATE_NOOP("action", "Enter third below")
             ),
    UiAction("interval-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fourth below"),
             QT_TRANSLATE_NOOP("action", "Enter fourth below")
             ),
    UiAction("interval-5",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fifth below"),
             QT_TRANSLATE_NOOP("action", "Enter fifth below")
             ),
    UiAction("interval-6",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sixth below"),
             QT_TRANSLATE_NOOP("action", "Enter sixth below")
             ),
    UiAction("interval-7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Seventh below"),
             QT_TRANSLATE_NOOP("action", "Enter seventh below")
             ),
    UiAction("interval-8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Octave below"),
             QT_TRANSLATE_NOOP("action", "Enter octave below")
             ),
    UiAction("interval-9",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ninth below"),
             QT_TRANSLATE_NOOP("action", "Enter ninth below")
             ),
    UiAction("note-c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "C"),
             QT_TRANSLATE_NOOP("action", "Enter note C")
             ),
    UiAction("note-d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "D"),
             QT_TRANSLATE_NOOP("action", "Enter note D")
             ),
    UiAction("note-e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "E"),
             QT_TRANSLATE_NOOP("action", "Enter note E")
             ),
    UiAction("note-f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "F"),
             QT_TRANSLATE_NOOP("action", "Enter note F")
             ),
    UiAction("note-g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "G"),
             QT_TRANSLATE_NOOP("action", "Enter note G")
             ),
    UiAction("note-a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "A"),
             QT_TRANSLATE_NOOP("action", "Enter note A")
             ),
    UiAction("note-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "B"),
             QT_TRANSLATE_NOOP("action", "Enter note B")
             ),
    UiAction("chord-c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add C to chord"),
             QT_TRANSLATE_NOOP("action", "Add note C to chord")
             ),
    UiAction("chord-d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add D to chord"),
             QT_TRANSLATE_NOOP("action", "Add note D to chord")
             ),
    UiAction("chord-e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add E to chord"),
             QT_TRANSLATE_NOOP("action", "Add note E to chord")
             ),
    UiAction("chord-f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add F to chord"),
             QT_TRANSLATE_NOOP("action", "Add note F to chord")
             ),
    UiAction("chord-g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add G to chord"),
             QT_TRANSLATE_NOOP("action", "Add note G to chord")
             ),
    UiAction("chord-a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add A to chord"),
             QT_TRANSLATE_NOOP("action", "Add note A to chord")
             ),
    UiAction("chord-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add B to chord"),
             QT_TRANSLATE_NOOP("action", "Add note B to chord")
             ),
    UiAction("insert-c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert C"),
             QT_TRANSLATE_NOOP("action", "Insert note C")
             ),
    UiAction("insert-d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert D"),
             QT_TRANSLATE_NOOP("action", "Insert note D")
             ),
    UiAction("insert-e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert E"),
             QT_TRANSLATE_NOOP("action", "Insert note E")
             ),
    UiAction("insert-f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert F"),
             QT_TRANSLATE_NOOP("action", "Insert note F")
             ),
    UiAction("insert-g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert G"),
             QT_TRANSLATE_NOOP("action", "Insert note G")
             ),
    UiAction("insert-a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert A"),
             QT_TRANSLATE_NOOP("action", "Insert note A")
             ),
    UiAction("insert-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert B"),
             QT_TRANSLATE_NOOP("action", "Insert note B")
             ),
    UiAction("rest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Rest"),
             QT_TRANSLATE_NOOP("action", "Enter rest")
             ),
    UiAction("rest-1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Whole rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Whole rest")
             ),
    UiAction("rest-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Half rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Half rest")
             ),
    UiAction("rest-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quarter rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Quarter rest")
             ),
    UiAction("rest-8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Eighth rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Eighth rest")
             ),
    UiAction("fret-0",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 0 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 0 on current string (TAB only)")
             ),
    UiAction("fret-1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 1 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 1 on current string (TAB only)")
             ),
    UiAction("fret-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 2 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 2 on current string (TAB only)")
             ),
    UiAction("fret-3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 3 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 3 on current string (TAB only)")
             ),
    UiAction("fret-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 4 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 4 on current string (TAB only)")
             ),
    UiAction("fret-5",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 5 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 5 on current string (TAB only)")
             ),
    UiAction("fret-6",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 6 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 6 on current string (TAB only)")
             ),
    UiAction("fret-7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 7 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 7 on current string (TAB only)")
             ),
    UiAction("fret-8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 8 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 8 on current string (TAB only)")
             ),
    UiAction("fret-9",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 9 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 9 on current string (TAB only)")
             ),
    UiAction("fret-10",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 10 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 10 on current string (TAB only)")
             ),
    UiAction("fret-11",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 11 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 11 on current string (TAB only)")
             ),
    UiAction("fret-12",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 12 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 12 on current string (TAB only)")
             ),
    UiAction("fret-13",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 13 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 13 on current string (TAB only)")
             ),
    UiAction("fret-14",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 14 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 14 on current string (TAB only)")
             ),
    UiAction("add-8va",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ottava 8va alta"),
             QT_TRANSLATE_NOOP("action", "Add ottava 8va alta")
             ),
    UiAction("add-8vb",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ottava 8va bassa"),
             QT_TRANSLATE_NOOP("action", "Add ottava 8va bassa")
             ),
    UiAction("add-hairpin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Crescendo"),
             QT_TRANSLATE_NOOP("action", "Add crescendo")
             ),
    UiAction("add-hairpin-reverse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Decrescendo"),
             QT_TRANSLATE_NOOP("action", "Add decrescendo")
             ),
    UiAction("add-noteline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Note anchored line")
             ),
    UiAction("chord-tie",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add tied note to chord")
             ),
    UiAction("title-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Title"),
             QT_TRANSLATE_NOOP("action", "Add title text")
             ),
    UiAction("subtitle-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Subtitle"),
             QT_TRANSLATE_NOOP("action", "Add subtitle text")
             ),
    UiAction("composer-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Composer"),
             QT_TRANSLATE_NOOP("action", "Add composer text")
             ),
    UiAction("poet-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Lyricist"),
             QT_TRANSLATE_NOOP("action", "Add lyricist text")
             ),
    UiAction("part-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Part name"),
             QT_TRANSLATE_NOOP("action", "Add part name")
             ),
    UiAction("system-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "System text"),
             QT_TRANSLATE_NOOP("action", "Add system text")
             ),
    UiAction("staff-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Staff text"),
             QT_TRANSLATE_NOOP("action", "Add staff text")
             ),
    UiAction("expression-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Expression text"),
             QT_TRANSLATE_NOOP("action", "Add expression text")
             ),
    UiAction("rehearsalmark-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Rehearsal mark"),
             QT_TRANSLATE_NOOP("action", "Add rehearsal mark")
             ),
    UiAction("instrument-change-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Instrument change"),
             QT_TRANSLATE_NOOP("action", "Add instrument change")
             ),
    UiAction("fingering-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fingering"),
             QT_TRANSLATE_NOOP("action", "Add fingering")
             ),
    UiAction("sticking-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sticking"),
             QT_TRANSLATE_NOOP("action", "Add sticking")
             ),
    UiAction("chord-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Chord symbol"),
             QT_TRANSLATE_NOOP("action", "Add chord symbol")
             ),
    UiAction("roman-numeral-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Roman numeral analysis"),
             QT_TRANSLATE_NOOP("action", "Add Roman numeral analysis")
             ),
    UiAction("nashville-number-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Nashville number"),
             QT_TRANSLATE_NOOP("action", "Add Nashville number")
             ),
    UiAction("lyrics",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Lyrics"),
             QT_TRANSLATE_NOOP("action", "Add lyrics")
             ),
    UiAction("figured-bass",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Figured bass"),
             QT_TRANSLATE_NOOP("action", "Add figured bass")
             ),
    UiAction("tempo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Tempo marking"),
             QT_TRANSLATE_NOOP("action", "Add tempo marking")
             ),
    UiAction("duplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Duplet"),
             QT_TRANSLATE_NOOP("action", "Add duplet")
             ),
    UiAction("triplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Triplet"),
             QT_TRANSLATE_NOOP("action", "Add triplet")
             ),
    UiAction("quadruplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quadruplet"),
             QT_TRANSLATE_NOOP("action", "Add quadruplet")
             ),
    UiAction("quintuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quintuplet"),
             QT_TRANSLATE_NOOP("action", "Add quintuplet")
             ),
    UiAction("sextuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sextuplet"),
             QT_TRANSLATE_NOOP("action", "Add sextuplet")
             ),
    UiAction("septuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Septuplet"),
             QT_TRANSLATE_NOOP("action", "Add septuplet")
             ),
    UiAction("octuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Octuplet"),
             QT_TRANSLATE_NOOP("action", "Add octuplet")
             ),
    UiAction("nonuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Nontuplet"),
             QT_TRANSLATE_NOOP("action", "Add nontuplet")
             ),
    UiAction("tuplet-dialog",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Other…"),
             QT_TRANSLATE_NOOP("action", "Other tuplets")
             ),
    UiAction("stretch-",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Decrease layout stretch"),
             QT_TRANSLATE_NOOP("action", "Decrease layout stretch factor of selected measures")
             ),
    UiAction("stretch+",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Increase layout stretch"),
             QT_TRANSLATE_NOOP("action", "Increase layout stretch factor of selected measures")
             ),
    UiAction("reset-stretch",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset layout stretch"),
             QT_TRANSLATE_NOOP("action", "Reset layout stretch factor of selected measures or entire score")
             ),
    UiAction("reset-text-style-overrides",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset text style overrides"),
             QT_TRANSLATE_NOOP("action", "Reset all text style overrides to default")
             ),
    UiAction("reset-beammode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset beams"),
             QT_TRANSLATE_NOOP("action", "Reset beams of selected measures")
             ),
    UiAction("reset",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset shapes and positions"),
             QT_TRANSLATE_NOOP("action", "Reset shapes and positions of selected elements to their defaults")
             ),
    UiAction("zoomin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Zoom in"),
             QT_TRANSLATE_NOOP("action", "Zoom in"),
             IconCode::Code::ZOOM_IN
             ),
    UiAction("zoomout",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Zoom out"),
             QT_TRANSLATE_NOOP("action", "Zoom out"),
             IconCode::Code::ZOOM_OUT
             ),
    UiAction("zoom100",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Zoom to 100%")
             ),
    UiAction("get-location",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Accessibility: Get location")
             ),
    UiAction("edit-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Edit element")
             ),
    UiAction("select-prev-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to beginning of measure")
             ),
    UiAction("select-next-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to end of measure")
             ),
    UiAction("select-begin-line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to beginning of line")
             ),
    UiAction("select-end-line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to end of line")
             ),
    UiAction("select-begin-score",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to beginning of score")
             ),
    UiAction("select-end-score",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to end of score")
             ),
    UiAction("select-staff-above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add staff above to selection")
             ),
    UiAction("select-staff-below",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add staff below to selection")
             ),
    UiAction("scr-prev",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Screen: Previous")
             ),
    UiAction("scr-next",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Screen: Next")
             ),
    UiAction("page-prev",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Previous")
             ),
    UiAction("page-next",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Next")
             ),
    UiAction("page-top",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Top of first")
             ),
    UiAction("page-end",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Bottom of last")
             ),
    UiAction("help",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Context sensitive help")
             ),
    UiAction("repeat-sel",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Repeat selection")
             ),
    UiAction("lock",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle score lock")
             ),
    UiAction("enh-both",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (both modes)"),
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (both modes)"),
             IconCode::Code::NONE
             ),
    UiAction("enh-current",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (current mode)"),
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (current mode)"),
             IconCode::Code::NONE
             ),
    UiAction("flip",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Flip direction"),
             QT_TRANSLATE_NOOP("action", "Flip direction"),
             IconCode::Code::NOTE_FLIP
             ),
    UiAction(TOGGLE_CONCERT_PITCH_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Concert pitch"),
             QT_TRANSLATE_NOOP("action", "Toggle 'Concert pitch'"),
             IconCode::Code::TUNING_FORK,
             Checkable::Yes
             ),
    UiAction("print",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Print"),
             QT_TRANSLATE_NOOP("action", "Print score/part"),
             IconCode::Code::PRINT
             ),
    UiAction("next-text-element",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Next text element"),
             QT_TRANSLATE_NOOP("action", "Move to text element on next note")
             ),
    UiAction("prev-text-element",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Previous text element"),
             QT_TRANSLATE_NOOP("action", "Move to text element on previous note")
             ),
    UiAction("next-beat-TEXT",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Next beat (Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Next beat (Chord symbol)")
             ),
    UiAction("prev-beat-TEXT",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Previous beat (Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Previous beat (Chord symbol)")
             ),
    UiAction("advance-longa",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Longa advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a longa (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-breve",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Breve advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a double whole note (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-1",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Whole note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a whole note (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-2",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Half note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a half note (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-4",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Quarter note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a quarter note (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-8",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Eighth note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of an eighth note (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-16",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "16th note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a 16th note (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-32",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "32nd note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a 32nd note (Figured bass/Chord symbol only)")
             ),
    UiAction("advance-64",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "64th note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a 64th note (Figured bass/Chord symbol only)")
             ),
    UiAction("next-lyric-verse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next lyric verse"),
             QT_TRANSLATE_NOOP("action", "Move to lyric in the next verse")
             ),
    UiAction("prev-lyric-verse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous lyric verse"),
             QT_TRANSLATE_NOOP("action", "Move to lyric in the previous verse")
             ),
    UiAction("next-syllable",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next syllable"),
             QT_TRANSLATE_NOOP("action", "Add hyphen and move to lyric on next note")
             ),
    UiAction("add-melisma",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add melisma"),
             QT_TRANSLATE_NOOP("action", "Add melisma line and move to lyric on next note")
             ),
    UiAction("add-lyric-verse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add lyric verse"),
             QT_TRANSLATE_NOOP("action", "Adds a new verse and starts editing")
             ),
    UiAction("text-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Bold face"),
             Checkable::Yes
             ),
    UiAction("text-i",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Italic"),
             Checkable::Yes
             ),
    UiAction("text-u",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Underline"),
             Checkable::Yes
             ),
    UiAction("text-s",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Strike"),
             Checkable::Yes
             ),
    UiAction("pitch-up-diatonic",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch up")
             ),
    UiAction("pitch-down-diatonic",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch down")
             ),
    UiAction("top-staff",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Go to top staff")
             ),
    UiAction("empty-trailing-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Go to first empty trailing measure")
             ),
    UiAction("mirror-note",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Mirror notehead")
             ),
    UiAction("add-trill",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle trill")
             ),
    UiAction("add-up-bow",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle up bow")
             ),
    UiAction("add-down-bow",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle down bow")
             ),
    UiAction("reset-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset style"),
             QT_TRANSLATE_NOOP("action", "Reset all style values to default")
             ),
    UiAction("clef-violin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add treble clef")
             ),
    UiAction("clef-bass",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add bass clef")
             ),
    UiAction("sharp2-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double ♯ (non-toggle)")
             ),
    UiAction("sharp-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "♯ (non-toggle)")
             ),
    UiAction("nat-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "♮ (non-toggle)")
             ),
    UiAction("flat-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "♭ (non-toggle)")
             ),
    UiAction("flat2-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double ♭ (non-toggle)")
             ),
    UiAction("transpose-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose up")
             ),
    UiAction("transpose-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose down")
             ),
    UiAction("pitch-up-diatonic-alterations",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch up (keep degree alterations)")
             ),
    UiAction("pitch-down-diatonic-alterations",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch down (keep degree alterations)")
             ),
    UiAction("transpose-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose down")
             ),
    UiAction("transpose-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose up")
             ),
    UiAction("full-measure-rest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Full measure rest")
             ),
    UiAction("toggle-mmrest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'Create multimeasure rest'")
             ),
    UiAction("toggle-hide-empty",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'Hide empty staves'")
             ),
    UiAction("set-visible",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Set visible")
             ),
    UiAction("unset-visible",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Set invisible")
             ),
    UiAction("toggle-autoplace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'automatic placement' for selected elements")
             ),
    UiAction("autoplace-enabled",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'automatic placement' (whole score)")
             ),
    UiAction("string-above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "String Above (TAB)")
             ),
    UiAction("string-below",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "String Below (TAB)")
             ),
    UiAction("pad-note-1-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Whole note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: whole note (TAB)"),
             IconCode::Code::NOTE_WHOLE
             ),
    UiAction("pad-note-2-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Half note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: half note (TAB)"),
             IconCode::Code::NOTE_HALF
             ),
    UiAction("pad-note-4-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quarter note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: quarter note (TAB)"),
             IconCode::Code::NOTE_QUARTER
             ),
    UiAction("pad-note-8-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "8th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 8th note (TAB)"),
             IconCode::Code::NOTE_8TH
             ),
    UiAction("pad-note-16-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "16th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 16th note (TAB)"),
             IconCode::Code::NOTE_16TH
             ),
    UiAction("pad-note-32-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "32nd note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 32nd note (TAB)"),
             IconCode::Code::NOTE_32ND
             ),
    UiAction("pad-note-64-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "64th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 64th note (TAB)"),
             IconCode::Code::NOTE_64TH
             ),
    UiAction("pad-note-128-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "128th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 128th note (TAB)"),
             IconCode::Code::NOTE_128TH
             ),
    UiAction("pad-note-256-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "256th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 256th note (TAB)"),
             IconCode::Code::NOTE_256TH
             ),
    UiAction("pad-note-512-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "512th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 512th note (TAB)"),
             IconCode::Code::NOTE_512TH
             ),
    UiAction("pad-note-1024-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "1024th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 1024th note (TAB)"),
             IconCode::Code::NOTE_1024TH
             ),
    UiAction("notation-context-menu",
             mu::context::UiCtxNotationFocused
             )
};

const UiActionList NotationUiActions::m_noteInputActions = {
    UiAction(NOTE_INPUT_ACTION_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Note input"),
             QT_TRANSLATE_NOOP("action", "Enter notes with a mouse or keyboard"),
             IconCode::Code::EDIT,
             Checkable::Yes
             ),
    UiAction("note-input-steptime",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Default (step time)"),
             QT_TRANSLATE_NOOP("action", "Enter notes with a mouse or keyboard"),
             IconCode::Code::EDIT
             ),
    UiAction("note-input-rhythm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Rhythm only (not pitch)"),
             QT_TRANSLATE_NOOP("action", "Enter durations with a single click or keypress"),
             IconCode::Code::RHYTHM_ONLY
             ),
    UiAction("note-input-repitch",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Re-pitch existing notes"),
             QT_TRANSLATE_NOOP("action", "Replace pitches without changing rhythms"),
             IconCode::Code::RE_PITH
             ),
    UiAction("note-input-realtime-auto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Real-time (metronome)"),
             QT_TRANSLATE_NOOP("action", "Enter notes at a fixed tempo indicated by a metronome beat"),
             IconCode::Code::METRONOME
             ),
    UiAction("note-input-realtime-manual",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Real-time (foot pedal)"),
             QT_TRANSLATE_NOOP("action", "Enter notes while tapping a key or pedal to set the tempo"),
             IconCode::Code::FOOT_PEDAL
             ),
    UiAction("note-input-timewise",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert"),
             QT_TRANSLATE_NOOP("action", "Insert notes by increasing measure duration"),
             IconCode::Code::NOTE_PLUS
             ),
    UiAction("note-longa",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Longa"),
             QT_TRANSLATE_NOOP("action", "Note duration: longa"),
             IconCode::Code::LONGO
             ),
    UiAction("note-breve",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double whole note"),
             QT_TRANSLATE_NOOP("action", "Note duration: double whole note"),
             IconCode::Code::NOTE_WHOLE_DOUBLE
             ),
    UiAction("pad-note-1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Whole note"),
             QT_TRANSLATE_NOOP("action", "Note duration: whole note"),
             IconCode::Code::NOTE_WHOLE
             ),
    UiAction("pad-note-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Half note"),
             QT_TRANSLATE_NOOP("action", "Note duration: half note"),
             IconCode::Code::NOTE_HALF
             ),
    UiAction("pad-note-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quarter note"),
             QT_TRANSLATE_NOOP("action", "Note duration: quarter note"),
             IconCode::Code::NOTE_QUARTER
             ),
    UiAction("pad-note-8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "8th note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 8th note"),
             IconCode::Code::NOTE_8TH
             ),
    UiAction("pad-note-16",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "16th note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 16th note"),
             IconCode::Code::NOTE_16TH
             ),
    UiAction("pad-note-32",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "32nd note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 32nd note"),
             IconCode::Code::NOTE_32ND
             ),
    UiAction("pad-note-64",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "64th note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 64th note"),
             IconCode::Code::NOTE_64TH
             ),
    UiAction("pad-note-128",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "128th note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 128th note"),
             IconCode::Code::NOTE_128TH
             ),
    UiAction("pad-note-256",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "256th note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 256th note"),
             IconCode::Code::NOTE_256TH
             ),
    UiAction("pad-note-512",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "512th note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 512th note"),
             IconCode::Code::NOTE_512TH
             ),
    UiAction("pad-note-1024",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "1024th note"),
             QT_TRANSLATE_NOOP("action", "Note duration: 1024th note"),
             IconCode::Code::NOTE_1024TH
             ),
    UiAction("pad-dot",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Augmentation dot"),
             QT_TRANSLATE_NOOP("action", "Note duration: augmentation dot"),
             IconCode::Code::NOTE_DOTTED
             ),
    UiAction("pad-dot2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double augmentation dot"),
             QT_TRANSLATE_NOOP("action", "Note duration: double augmentation dot"),
             IconCode::Code::NOTE_DOTTED_2
             ),
    UiAction("pad-dot3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Triple augmentation dot"),
             QT_TRANSLATE_NOOP("action", "Note duration: triple augmentation dot"),
             IconCode::Code::NOTE_DOTTED_3
             ),
    UiAction("pad-dot4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quadruple augmentation dot"),
             QT_TRANSLATE_NOOP("action", "Note duration: quadruple augmentation dot"),
             IconCode::Code::NOTE_DOTTED_4
             ),
    UiAction("pad-rest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Rest"),
             QT_TRANSLATE_NOOP("action", "Note input: rest"),
             IconCode::Code::REST
             ),
    UiAction("next-segment-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Accessibility: Next segment element")
             ),
    UiAction("prev-segment-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Accessibility: Previous segment element")
             ),
    UiAction("flat",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Flat"),
             QT_TRANSLATE_NOOP("action", "Toggle accidental: Flat"),
             IconCode::Code::FLAT
             ),
    UiAction("flat2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double flat"),
             QT_TRANSLATE_NOOP("action", "Toggle accidental: Double flat"),
             IconCode::Code::FLAT_DOUBLE
             ),
    UiAction("nat",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Natural"),
             QT_TRANSLATE_NOOP("action", "Toggle accidental: Natural"),
             IconCode::Code::NATURAL
             ),
    UiAction("sharp",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sharp"),
             QT_TRANSLATE_NOOP("action", "Toggle accidental: Sharp"),
             IconCode::Code::SHARP
             ),
    UiAction("sharp2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double sharp"),
             QT_TRANSLATE_NOOP("action", "Toggle accidental: Double sharp"),
             IconCode::Code::SHARP_DOUBLE
             ),
    UiAction("tie",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Tie"),
             QT_TRANSLATE_NOOP("action", "Note duration: Tie"),
             IconCode::Code::NOTE_TIE
             ),
    UiAction("add-slur",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Slur"),
             QT_TRANSLATE_NOOP("action", "Add slur"),
             IconCode::Code::NOTE_SLUR
             ),
    UiAction("add-marcato",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Marcato"),
             QT_TRANSLATE_NOOP("action", "Toggle marcato"),
             IconCode::Code::MARCATO
             ),
    UiAction("add-sforzato",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Accent"),
             QT_TRANSLATE_NOOP("action", "Toggle accent"),
             IconCode::Code::ACCENT
             ),
    UiAction("add-tenuto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Tenuto"),
             QT_TRANSLATE_NOOP("action", "Toggle tenuto"),
             IconCode::Code::TENUTO
             ),
    UiAction("add-staccato",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Staccato"),
             QT_TRANSLATE_NOOP("action", "Toggle staccato"),
             IconCode::Code::STACCATO
             ),
    UiAction("cross-staff-beaming",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Cross-staff beaming"),
             QT_TRANSLATE_NOOP("action", "Move notes to staff above or below"),
             IconCode::Code::CROSS_STAFF_BEAMING
             ),
    UiAction("tuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Tuplet"),
             QT_TRANSLATE_NOOP("action", "Add tuplet"),
             IconCode::Code::NOTE_TUPLET
             ),
    UiAction("voice-1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Voice 1"),
             QT_TRANSLATE_NOOP("action", "Voice 1"),
             IconCode::Code::VOICE_1
             ),
    UiAction("voice-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Voice 2"),
             QT_TRANSLATE_NOOP("action", "Voice 2"),
             IconCode::Code::VOICE_2
             ),
    UiAction("voice-3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Voice 3"),
             QT_TRANSLATE_NOOP("action", "Voice 3"),
             IconCode::Code::VOICE_3
             ),
    UiAction("voice-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Voice 4"),
             QT_TRANSLATE_NOOP("action", "Voice 4"),
             IconCode::Code::VOICE_4
             )
};

const UiActionList NotationUiActions::m_scoreConfigActions = {
    UiAction(SHOW_INVISIBLE_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show invisible"),
             QT_TRANSLATE_NOOP("action", "Show invisible"),
             Checkable::Yes
             ),
    UiAction(SHOW_UNPRINTABLE_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show formatting"),
             QT_TRANSLATE_NOOP("action", "Show formatting"),
             Checkable::Yes
             ),
    UiAction(SHOW_FRAMES_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show frames"),
             QT_TRANSLATE_NOOP("action", "Show frames"),
             Checkable::Yes
             ),
    UiAction(SHOW_PAGEBORDERS_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show page margins"),
             QT_TRANSLATE_NOOP("action", "Show page margins"),
             Checkable::Yes
             ),
    UiAction(SHOW_IRREGULAR_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Mark irregular measures"),
             QT_TRANSLATE_NOOP("action", "Mark irregular measures"),
             Checkable::Yes
             )
};

const UiActionList NotationUiActions::m_engravingDebuggingActions = {
    UiAction("show-skylines",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show skylines"),
             Checkable::Yes
             ),
    UiAction("show-segment-shapes",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show segment shapes"),
             Checkable::Yes
             ),
    UiAction("show-bounding-rect",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show bounding rectangles"),
             Checkable::Yes
             ),
    UiAction("show-system-bounding-rect",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show system bounding rectangles"),
             Checkable::Yes
             ),
    UiAction("show-corrupted-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show corrupted measures"),
             Checkable::Yes
             )
};

NotationUiActions::NotationUiActions(std::shared_ptr<NotationActionController> controller)
    : m_controller(controller)
{
}

void NotationUiActions::init()
{
    m_controller->currentNotationNoteInputChanged().onNotify(this, [this]() {
        m_actionCheckedChanged.send({ NOTE_INPUT_ACTION_CODE });
    });

    m_controller->currentNotationChanged().onNotify(this, [this]() {
        actions::ActionCodeList actions;
        for (const UiAction& action : m_scoreConfigActions) {
            actions.push_back(action.code);
        }
        m_actionCheckedChanged.send(actions);

        if (m_controller->currentNotationInteraction()) {
            m_controller->currentNotationInteraction()->scoreConfigChanged().onReceive(this, [this](ScoreConfigType configType) {
                static const std::unordered_map<ScoreConfigType, std::string> configActions = {
                    { ScoreConfigType::ShowInvisibleElements, SHOW_INVISIBLE_CODE },
                    { ScoreConfigType::ShowUnprintableElements, SHOW_UNPRINTABLE_CODE },
                    { ScoreConfigType::ShowFrames, SHOW_FRAMES_CODE },
                    { ScoreConfigType::ShowPageMargins, SHOW_PAGEBORDERS_CODE },
                    { ScoreConfigType::MarkIrregularMeasures, SHOW_IRREGULAR_CODE }
                };

                m_actionCheckedChanged.send({ configActions.at(configType) });
            });
        }
        m_controller->currentNotationStyleChanged().onNotify(this, [this]() {
            m_actionCheckedChanged.send({ TOGGLE_CONCERT_PITCH_CODE });
        });
    });

    engravingConfiguration()->debuggingOptionsChanged().onNotify(this, [this]() {
        actions::ActionCodeList actions;
        for (const UiAction& action : m_engravingDebuggingActions) {
            actions.push_back(action.code);
        }
        m_actionCheckedChanged.send(actions);
    });
}

const UiActionList& NotationUiActions::actionsList() const
{
    static UiActionList alist;
    if (alist.empty()) {
        alist.insert(alist.end(), m_actions.begin(), m_actions.end());
        alist.insert(alist.end(), m_noteInputActions.begin(), m_noteInputActions.end());
        alist.insert(alist.end(), m_scoreConfigActions.begin(), m_scoreConfigActions.end());
        alist.insert(alist.end(), m_engravingDebuggingActions.begin(), m_engravingDebuggingActions.end());
    }
    return alist;
}

bool NotationUiActions::actionEnabled(const UiAction& act) const
{
    if (!m_controller->canReceiveAction(act.code)) {
        return false;
    }

    return true;
}

bool NotationUiActions::isScoreConfigAction(const actions::ActionCode& code) const
{
    for (const UiAction& a : m_scoreConfigActions) {
        if (a.code == code) {
            return true;
        }
    }
    return false;
}

bool NotationUiActions::isScoreConfigChecked(const actions::ActionCode& code, const ScoreConfig& cfg) const
{
    if (SHOW_INVISIBLE_CODE == code) {
        return cfg.isShowInvisibleElements;
    }
    if (SHOW_UNPRINTABLE_CODE == code) {
        return cfg.isShowUnprintableElements;
    }
    if (SHOW_FRAMES_CODE == code) {
        return cfg.isShowFrames;
    }
    if (SHOW_PAGEBORDERS_CODE == code) {
        return cfg.isShowPageMargins;
    }
    if (SHOW_IRREGULAR_CODE == code) {
        return cfg.isMarkIrregularMeasures;
    }

    return false;
}

bool NotationUiActions::actionChecked(const UiAction& act) const
{
    if (act.code == NOTE_INPUT_ACTION_CODE) {
        auto noteInput = m_controller->currentNotationNoteInput();
        if (noteInput) {
            return noteInput->isNoteInputMode();
        }
    }

    if (act.code == TOGGLE_CONCERT_PITCH_CODE) {
        auto style = m_controller->currentNotationStyle();
        if (style) {
            return style->styleValue(StyleId::concertPitch).toBool();
        }
    }

    if (isScoreConfigAction(act.code)) {
        auto interaction = m_controller->currentNotationInteraction();
        if (interaction) {
            return isScoreConfigChecked(act.code, interaction->scoreConfig());
        }
    }

    auto engravingDebuggingActionsSearch = NotationActionController::engravingDebuggingActions.find(act.code);
    if (engravingDebuggingActionsSearch != NotationActionController::engravingDebuggingActions.cend()) {
        return engravingConfiguration()->debuggingOptions().*(engravingDebuggingActionsSearch->second);
    }

    return false;
}

mu::async::Channel<mu::actions::ActionCodeList> NotationUiActions::actionEnabledChanged() const
{
    return m_actionEnabledChanged;
}

mu::async::Channel<mu::actions::ActionCodeList> NotationUiActions::actionCheckedChanged() const
{
    return m_actionCheckedChanged;
}

UiActionList NotationUiActions::defaultNoteInputActions()
{
    return m_noteInputActions;
}

DurationType NotationUiActions::actionDurationType(const ActionCode& actionCode)
{
    static QMap<actions::ActionCode, DurationType> durations = {
        { "note-longa", DurationType::V_LONG },
        { "note-breve", DurationType::V_BREVE },
        { "pad-note-1", DurationType::V_WHOLE },
        { "pad-note-2", DurationType::V_HALF },
        { "pad-note-4", DurationType::V_QUARTER },
        { "pad-note-8", DurationType::V_EIGHTH },
        { "pad-note-16", DurationType::V_16TH },
        { "pad-note-32", DurationType::V_32ND },
        { "pad-note-64", DurationType::V_64TH },
        { "pad-note-128", DurationType::V_128TH },
        { "pad-note-256", DurationType::V_256TH },
        { "pad-note-512", DurationType::V_512TH },
        { "pad-note-1024", DurationType::V_1024TH }
    };

    DurationType type = DurationType::V_INVALID;
    if (durations.contains(actionCode)) {
        type = durations[actionCode];
    }

    return type;
}

AccidentalType NotationUiActions::actionAccidentalType(const ActionCode& actionCode)
{
    static QMap<actions::ActionCode, AccidentalType> accidentals = {
        { "flat2", AccidentalType::FLAT2 },
        { "flat", AccidentalType::FLAT },
        { "nat", AccidentalType::NATURAL },
        { "sharp", AccidentalType::SHARP },
        { "sharp2", AccidentalType::SHARP2 }
    };

    AccidentalType type = AccidentalType::NONE;
    if (accidentals.contains(actionCode)) {
        type = accidentals[actionCode];
    }

    return type;
}

int NotationUiActions::actionDotCount(const ActionCode& actionCode)
{
    static QMap<actions::ActionCode, int> dots = {
        { "pad-dot", 1 },
        { "pad-dot2", 2 },
        { "pad-dot3", 3 },
        { "pad-dot4", 4 }
    };

    int dotCount = 0;
    if (dots.contains(actionCode)) {
        dotCount = dots[actionCode];
    }

    return dotCount;
}

int NotationUiActions::actionVoice(const ActionCode& actionCode)
{
    QMap<actions::ActionCode, int> voices {
        { "voice-1", 0 },
        { "voice-2", 1 },
        { "voice-3", 2 },
        { "voice-4", 3 }
    };

    int voice = 0;
    if (voices.contains(actionCode)) {
        voice = voices[actionCode];
    }

    return voice;
}

SymbolId NotationUiActions::actionArticulationSymbolId(const ActionCode& actionCode)
{
    static QMap<actions::ActionCode, SymbolId> articulations {
        { "add-marcato", SymbolId::articMarcatoAbove },
        { "add-sforzato", SymbolId::articAccentAbove },
        { "add-tenuto", SymbolId::articTenutoAbove },
        { "add-staccato", SymbolId::articStaccatoAbove }
    };

    SymbolId symbolId = SymbolId::noSym;
    if (articulations.contains(actionCode)) {
        symbolId = articulations[actionCode];
    }

    return symbolId;
}

const mu::ui::ToolConfig& NotationUiActions::defaultNoteInputBarConfig()
{
    static ToolConfig config;
    if (!config.isValid()) {
        config.items = {
            { "note-input", true },
            { "pad-note-1024", false },
            { "pad-note-512", false },
            { "pad-note-256", false },
            { "pad-note-128", false },
            { "pad-note-64", true },
            { "pad-note-32", true },
            { "pad-note-16", true },
            { "pad-note-8", true },
            { "pad-note-4", true },
            { "pad-note-2", true },
            { "pad-note-1", true },
            { "note-breve", false },
            { "note-longa", false },
            { "", true },
            { "pad-dot", true },
            { "pad-dot2", false },
            { "pad-dot3", false },
            { "pad-dot4", false },
            { "pad-rest", true },
            { "", true },
            { "flat2", true },
            { "flat", true },
            { "nat", true },
            { "sharp", true },
            { "sharp2", true },
            { "", true },
            { "tie", true },
            { "add-slur", true },
            { "", true },
            { "add-marcato", true },
            { "add-sforzato", true },
            { "add-tenuto", true },
            { "add-staccato", true },
            { "", true },
            { "cross-staff-beaming", false },
            { "tuplet", true },
            { "flip", true },
            { "", true },
            { "voice-1", true },
            { "voice-2", true },
            { "voice-3", false },
            { "voice-4", false }
        };
    }
    return config;
}
