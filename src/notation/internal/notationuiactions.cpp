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

// avoid translation duplication

// //: This is comment for translator

//: Note
static const QString noteC(QT_TRANSLATE_NOOP("action", "C"));
//: Note
static const QString noteD(QT_TRANSLATE_NOOP("action", "D"));
//: Note
static const QString noteE(QT_TRANSLATE_NOOP("action", "E"));
//: Note
static const QString noteF(QT_TRANSLATE_NOOP("action", "F"));
//: Note
static const QString noteG(QT_TRANSLATE_NOOP("action", "G"));
//: Note
static const QString noteA(QT_TRANSLATE_NOOP("action", "A"));
//: Note
static const QString noteB(QT_TRANSLATE_NOOP("action", "B"));

static const QString Enter_note_X(QT_TRANSLATE_NOOP("action", "Enter note %1"));
static const QString Add_X_to_chord(QT_TRANSLATE_NOOP("action", "Add %1 to chord"));
static const QString Add_note_X_to_chord(QT_TRANSLATE_NOOP("action", "Add note %1 to chord"));
static const QString Insert_X(QT_TRANSLATE_NOOP("action", "Insert %1"));
static const QString Insert_note_X(QT_TRANSLATE_NOOP("action", "Insert note %1"));

//! NOTE Each notation actions should has context is UiCtxNotationOpened.
//! If you want what action to dispatch by shortcut only when notation is focused (ex notation-move-right by press Right key),
//! then you should set the shortcut context accordingly, not the action context.
//! Because actions can be dispatched not only shortcuts, but another way, ex by click Button, Menu and etc

const UiActionList NotationUiActions::m_actions = {
    UiAction("notation-escape",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Esc"),
             ActionCategory::System
             ),
    UiAction("put-note", // args: PointF pos, bool replace, bool insert
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Put note"),
             ActionCategory::NoteInput
             ),
    UiAction("remove-note", // args: PointF pos
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Remove note"),
             ActionCategory::NoteInput
             ),
    UiAction("next-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next element"),
             QT_TRANSLATE_NOOP("action", "Accessibility: Next element"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("prev-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous element"),
             QT_TRANSLATE_NOOP("action", "Accessibility: Previous element"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("notation-move-right",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next chord / Shift text right"),
             QT_TRANSLATE_NOOP("action", "Go to next chord or shift text right"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("notation-move-left",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous chord / Shift text left"),
             QT_TRANSLATE_NOOP("action", "Go to previous chord or shift text left"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("notation-move-right-quickly",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next measure / Shift text right quickly"),
             QT_TRANSLATE_NOOP("action", "Go to next measure or shift text right quickly"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("notation-move-left-quickly",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous measure / Shift text left quickly"),
             QT_TRANSLATE_NOOP("action", "Go to previous measure or shift text left quickly"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("up-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Up note in chord"),
             QT_TRANSLATE_NOOP("action", "Go to higher pitched note in chord"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("down-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Down note in chord"),
             QT_TRANSLATE_NOOP("action", "Go to lower pitched note in chord"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("top-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Top note in chord"),
             QT_TRANSLATE_NOOP("action", "Go to top note in chord"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("bottom-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Bottom note in chord"),
             QT_TRANSLATE_NOOP("action", "Go bottom note in chord"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("first-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "First element"),
             QT_TRANSLATE_NOOP("action", "Go to first element in score")
             ),
    UiAction("last-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Last element"),
             QT_TRANSLATE_NOOP("action", "Go to last element in score"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("move-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move to staff above"),
             QT_TRANSLATE_NOOP("action", "Move chord/rest to staff above"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("move-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move to staff below"),
             QT_TRANSLATE_NOOP("action", "Move chord/rest to staff below"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("next-track",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next staff or voice"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("prev-track",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous staff or voice"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("next-frame",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next frame"),
             ActionCategory::Playback
             ),
    UiAction("prev-frame",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous frame"),
             ActionCategory::Playback
             ),
    UiAction("next-system",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next system"),
             ActionCategory::Undefined
             ),
    UiAction("prev-system",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous system"),
             ActionCategory::Undefined
             ),
    UiAction("show-irregular",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Mark irregular measures"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("toggle-insert-mode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'insert mode'"),
             ActionCategory::NoteInput
             ),
    UiAction("select-next-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add next chord to selection"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-prev-chord",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add previous chord to selection"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("move-left",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move chord/rest left"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("move-right",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Move chord/rest right"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("pitch-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Up"),
             QT_TRANSLATE_NOOP("action", "Pitch up or move text or articulation up"),
             ActionCategory::Articulations
             ),
    UiAction("pitch-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Down"),
             QT_TRANSLATE_NOOP("action", "Pitch down or move text or articulation down"),
             ActionCategory::Articulations
             ),
    UiAction("pitch-down-octave",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Down octave"),
             QT_TRANSLATE_NOOP("action", "Pitch down by an octave or move text or articulation down"),
             ActionCategory::Articulations
             ),
    UiAction("pitch-up-octave",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Up octave"),
             QT_TRANSLATE_NOOP("action", "Pitch up by an octave or move text or articulation up"),
             ActionCategory::Articulations
             ),
    UiAction("double-duration",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double duration"),
             ActionCategory::Articulations
             ),
    UiAction("half-duration",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Halve duration"),
             ActionCategory::Articulations
             ),
    UiAction("inc-duration-dotted",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double selected duration (dotted)"),
             ActionCategory::Articulations
             ),
    UiAction("dec-duration-dotted",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Halve selected duration (dotted)"),
             ActionCategory::Articulations
             ),
    UiAction("notation-cut",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Cut"),
             IconCode::Code::CUT,
             ActionCategory::System
             ),
    UiAction("notation-copy",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Copy"),
             IconCode::Code::COPY,
             ActionCategory::System
             ),
    UiAction("notation-paste",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste"),
             IconCode::Code::PASTE,
             ActionCategory::System
             ),
    UiAction("notation-paste-half",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste half duration"),
             ActionCategory::System
             ),
    UiAction("notation-paste-double",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste double duration"),
             ActionCategory::System
             ),
    UiAction("notation-paste-special",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Paste special"),
             ActionCategory::System
             ),
    UiAction("notation-swap",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Swap with clipboard"),
             ActionCategory::System
             ),
    UiAction("toggle-visible",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle visibility of elements"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("notation-select-all",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select all"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("notation-select-section",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select section"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-similar",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Similar"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-similar-staff",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Similar on this staff"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements on the same staff"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-similar-range",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Similar in this range"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements in the range selection"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-dialog",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "More…"),
             QT_TRANSLATE_NOOP("action", "Select all similar elements with more options"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("notation-delete",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Delete"),
             QT_TRANSLATE_NOOP("action", "Delete the selected element(s)"),
             IconCode::Code::DELETE_TANK,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("edit-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Style…"),
             QT_TRANSLATE_NOOP("action", "Edit style"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("page-settings",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page settings…"),
             QT_TRANSLATE_NOOP("action", "Page settings"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("load-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Load style…"),
             QT_TRANSLATE_NOOP("action", "Load style"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("save-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Save style…"),
             QT_TRANSLATE_NOOP("action", "Save style"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("transpose",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "&Transpose…"),
             QT_TRANSLATE_NOOP("action", "Transpose"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("explode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Explode"),
             QT_TRANSLATE_NOOP("action", "Explode contents of top selected staff into staves below"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("implode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Implode"),
             QT_TRANSLATE_NOOP("action", "Implode contents of selected staves into top selected staff"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("realize-chord-symbols",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Realize chord symbols"),
             QT_TRANSLATE_NOOP("action", "Convert chord symbols into notes"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("time-delete",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Remove selected range"),
             IconCode::Code::DELETE_TANK,
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("slash-fill",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fill with slashes"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("slash-rhythm",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'rhythmic slash notation'"),
             ActionCategory::NoteInput
             ),
    UiAction("pitch-spell",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Respell pitches"),
             ActionCategory::Articulations
             ),
    UiAction("reset-groupings",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Regroup rhythms"),
             QT_TRANSLATE_NOOP("action", "Combine rests and tied notes from selection and resplit at rhythmical boundaries"),
             ActionCategory::Articulations
             ),
    UiAction("resequence-rehearsal-marks",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Resequence rehearsal marks"),
             ActionCategory::Articulations
             ),
    UiAction("unroll-repeats",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Unroll repeats"),
             ActionCategory::Articulations
             ),
    UiAction("copy-lyrics-to-clipboard",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Copy lyrics to clipboard"),
             ActionCategory::System
             ),
    UiAction("del-empty-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Remove empty trailing measures"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("parts",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Parts"),
             QT_TRANSLATE_NOOP("action", "Manage parts"),
             IconCode::Code::PAGE,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("view-mode-page",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page view"),
             IconCode::Code::PAGE_VIEW,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("view-mode-continuous",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Continuous view (horizontal)"),
             IconCode::Code::CONTINUOUS_VIEW,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("view-mode-single",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Continuous view (vertical)"),
             IconCode::Code::CONTINUOUS_VIEW_VERTICAL,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("find",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Find / Go To"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("staff-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Staff/part properties…"),
             QT_TRANSLATE_NOOP("action", "Staff/part properties"),
             ActionCategory::DialogsAndWindows
             ),
    UiAction("staff-text-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Staff text properties…"),
             QT_TRANSLATE_NOOP("action", "Staff text properties"),
             ActionCategory::DialogsAndWindows
             ),
    UiAction("system-text-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "System text properties…"),
             QT_TRANSLATE_NOOP("action", "System text properties"),
             ActionCategory::DialogsAndWindows
             ),
    UiAction("measure-properties",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Measure properties…"),
             QT_TRANSLATE_NOOP("action", "Measure properties"),
             ActionCategory::DialogsAndWindows
             ),
    UiAction("add-remove-breaks",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add/remove system breaks…"),
             QT_TRANSLATE_NOOP("action", "Add/remove system breaks"),
             ActionCategory::Articulations
             ),
    UiAction("undo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Undo"),
             QT_TRANSLATE_NOOP("action", "Undo last change"),
             IconCode::Code::UNDO,
             ActionCategory::System
             ),
    UiAction("redo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Redo"),
             QT_TRANSLATE_NOOP("action", "Redo last undo"),
             IconCode::Code::REDO,
             ActionCategory::System
             ),
    UiAction("voice-x12",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-2"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-2"),
             ActionCategory::Articulations
             ),
    UiAction("voice-x13",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-3"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-3"),
             ActionCategory::Articulations
             ),
    UiAction("voice-x14",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-4"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 1-4"),
             ActionCategory::Articulations
             ),
    UiAction("voice-x23",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-3"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-3"),
             ActionCategory::Articulations
             ),
    UiAction("voice-x24",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-4"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 2-4"),
             ActionCategory::Articulations
             ),
    UiAction("voice-x34",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Exchange voice 3-4"),
             QT_TRANSLATE_NOOP("action", "Exchange voice 3-4"),
             ActionCategory::Articulations
             ),
    UiAction("system-break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle system break"),
             QT_TRANSLATE_NOOP("action", "Toggle 'system break'"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("page-break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle page break"),
             QT_TRANSLATE_NOOP("action", "Toggle 'page break'"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("section-break",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle section break"),
             QT_TRANSLATE_NOOP("action", "Toggle 'section break'"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("split-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Split measure before selected note/rest"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("join-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Join selected measures"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("insert-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert one measure before selection"),
             IconCode::Code::INSERT_ONE_MEASURE,
             ActionCategory::NoteInput
             ),
    UiAction("insert-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures before selection…"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-measures-after-selection",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures after selection…"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-measures-at-start-of-score",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures at start of score…"),
             ActionCategory::NoteInput
             ),
    UiAction("append-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert one measure at end of score"),
             ActionCategory::NoteInput
             ),
    UiAction("append-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert measures at end of score…"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-hbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert horizontal frame"),
             IconCode::Code::HORIZONTAL_FRAME,
             ActionCategory::Internal

             ),
    UiAction("insert-vbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert vertical frame"),
             IconCode::Code::VERTICAL_FRAME,
             ActionCategory::Internal
             ),
    UiAction("insert-textframe",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert text frame"),
             IconCode::Code::TEXT_FRAME,
             ActionCategory::Internal
             ),
    UiAction("append-hbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Append horizontal frame"),
             ActionCategory::Internal
             ),
    UiAction("append-vbox",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Append vertical frame"),
             ActionCategory::Internal
             ),
    UiAction("append-textframe",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Append text frame"),
             ActionCategory::Internal
             ),
    UiAction("acciaccatura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Acciaccatura"),
             IconCode::Code::ACCIACCATURA,
             ActionCategory::NoteInput
             ),
    UiAction("appoggiatura",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Appoggiatura"),
             IconCode::Code::APPOGGIATURA,
             ActionCategory::NoteInput
             ),
    UiAction("grace4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: quarter"),
             IconCode::Code::GRACE4,
             ActionCategory::NoteInput
             ),
    UiAction("grace16",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 16th"),
             IconCode::Code::GRACE16,
             ActionCategory::NoteInput
             ),
    UiAction("grace32",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 32nd"),
             IconCode::Code::GRACE32,
             ActionCategory::NoteInput
             ),
    UiAction("grace8after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 8th after"),
             IconCode::Code::GRACE8_AFTER,
             ActionCategory::NoteInput
             ),
    UiAction("grace16after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 16th after"),
             IconCode::Code::GRACE16_AFTER,
             ActionCategory::NoteInput
             ),
    UiAction("grace32after",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Grace: 32nd after"),
             IconCode::Code::GRACE32_AFTER,
             ActionCategory::NoteInput
             ),
    UiAction("beam-auto",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Auto beam"),
             IconCode::Code::AUTO_TEXT,
             ActionCategory::Articulations
             ),
    UiAction("beam-none",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "No beam"),
             IconCode::Code::NOTE_HEAD_EIGHTH,
             ActionCategory::Articulations
             ),
    UiAction("beam-break-left",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Break beam left"),
             IconCode::Code::BEAM_BREAK_LEFT,
             ActionCategory::Articulations
             ),
    UiAction("beam-break-inner-8th",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Break inner beams (8th)"),
             IconCode::Code::BEAM_BREAK_INNER_8TH,
             ActionCategory::Articulations
             ),
    UiAction("beam-break-inner-16th",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Break inner beams (16th)"),
             IconCode::Code::BEAM_BREAK_INNER_16TH,
             ActionCategory::Articulations
             ),
    UiAction("beam-join",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Join beams"),
             IconCode::Code::BEAM_JOIN,
             ActionCategory::Articulations
             ),
    UiAction("beam-feathered-decelerate",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Feathered beam, decelerate"),
             IconCode::Code::BEAM_FEATHERED_DECELERATE,
             ActionCategory::Articulations
             ),
    UiAction("beam-feathered-accelerate",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Feathered beam, accelerate"),
             IconCode::Code::BEAM_FEATHERED_ACCELERATE,
             ActionCategory::Articulations
             ),
    UiAction("add-brackets",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add brackets to accidental"),
             IconCode::Code::BRACKET,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("add-braces",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add braces to element"),
             IconCode::Code::BRACE,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("add-parentheses",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add parentheses to element"),
             IconCode::Code::BRACKET_PARENTHESES,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("interval1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Unison above"),
             QT_TRANSLATE_NOOP("action", "Enter unison above"),
             ActionCategory::Articulations
             ),
    UiAction("interval2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Second above"),
             QT_TRANSLATE_NOOP("action", "Enter second above"),
             ActionCategory::Articulations
             ),
    UiAction("interval3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Third above"),
             QT_TRANSLATE_NOOP("action", "Enter third above"),
             ActionCategory::Articulations
             ),
    UiAction("interval4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fourth above"),
             QT_TRANSLATE_NOOP("action", "Enter fourth above"),
             ActionCategory::Articulations
             ),
    UiAction("interval5",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fifth above"),
             QT_TRANSLATE_NOOP("action", "Enter fifth above"),
             ActionCategory::Articulations
             ),
    UiAction("interval6",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sixth above"),
             QT_TRANSLATE_NOOP("action", "Enter sixth above"),
             ActionCategory::Articulations
             ),
    UiAction("interval7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Seventh above"),
             QT_TRANSLATE_NOOP("action", "Enter seventh above"),
             ActionCategory::Articulations
             ),
    UiAction("interval8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Octave above"),
             QT_TRANSLATE_NOOP("action", "Enter octave above"),
             ActionCategory::Articulations
             ),
    UiAction("interval9",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ninth above"),
             QT_TRANSLATE_NOOP("action", "Enter ninth above"),
             ActionCategory::Articulations
             ),
    UiAction("interval-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Second below"),
             QT_TRANSLATE_NOOP("action", "Enter second below"),
             ActionCategory::Articulations
             ),
    UiAction("interval-3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Third below"),
             QT_TRANSLATE_NOOP("action", "Enter third below"),
             ActionCategory::Articulations
             ),
    UiAction("interval-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fourth below"),
             QT_TRANSLATE_NOOP("action", "Enter fourth below"),
             ActionCategory::Articulations
             ),
    UiAction("interval-5",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fifth below"),
             QT_TRANSLATE_NOOP("action", "Enter fifth below"),
             ActionCategory::Articulations
             ),
    UiAction("interval-6",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sixth below"),
             QT_TRANSLATE_NOOP("action", "Enter sixth below"),
             ActionCategory::Articulations
             ),
    UiAction("interval-7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Seventh below"),
             QT_TRANSLATE_NOOP("action", "Enter seventh below"),
             ActionCategory::Articulations
             ),
    UiAction("interval-8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Octave below"),
             QT_TRANSLATE_NOOP("action", "Enter octave below"),
             ActionCategory::Articulations
             ),
    UiAction("interval-9",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ninth below"),
             QT_TRANSLATE_NOOP("action", "Enter ninth below"),
             ActionCategory::Articulations
             ),
    UiAction("note-c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "C"),
             QT_TRANSLATE_NOOP("action", "Enter note C"),
             ActionCategory::NoteInput
             ),
    UiAction("note-d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "D"),
             QT_TRANSLATE_NOOP("action", "Enter note D"),
             ActionCategory::NoteInput
             ),
    UiAction("note-e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "E"),
             QT_TRANSLATE_NOOP("action", "Enter note E"),
             ActionCategory::NoteInput
             ),
    UiAction("note-f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "F"),
             QT_TRANSLATE_NOOP("action", "Enter note F"),
             ActionCategory::NoteInput
             ),
    UiAction("note-g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "G"),
             QT_TRANSLATE_NOOP("action", "Enter note G"),
             ActionCategory::NoteInput
             ),
    UiAction("note-a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "A"),
             QT_TRANSLATE_NOOP("action", "Enter note A"),
             ActionCategory::NoteInput
             ),
    UiAction("note-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "B"),
             QT_TRANSLATE_NOOP("action", "Enter note B"),
             ActionCategory::NoteInput
             ),
    UiAction("chord-c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add C to chord"),
             QT_TRANSLATE_NOOP("action", "Add note C to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("chord-d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add D to chord"),
             QT_TRANSLATE_NOOP("action", "Add note D to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("chord-e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add E to chord"),
             QT_TRANSLATE_NOOP("action", "Add note E to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("chord-f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add F to chord"),
             QT_TRANSLATE_NOOP("action", "Add note F to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("chord-g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add G to chord"),
             QT_TRANSLATE_NOOP("action", "Add note G to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("chord-a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add A to chord"),
             QT_TRANSLATE_NOOP("action", "Add note A to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("chord-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add B to chord"),
             QT_TRANSLATE_NOOP("action", "Add note B to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-c",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert C"),
             QT_TRANSLATE_NOOP("action", "Insert note C"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-d",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert D"),
             QT_TRANSLATE_NOOP("action", "Insert note D"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-e",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert E"),
             QT_TRANSLATE_NOOP("action", "Insert note E"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-f",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert F"),
             QT_TRANSLATE_NOOP("action", "Insert note F"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-g",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert G"),
             QT_TRANSLATE_NOOP("action", "Insert note G"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-a",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert A"),
             QT_TRANSLATE_NOOP("action", "Insert note A"),
             ActionCategory::NoteInput
             ),
    UiAction("insert-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Insert B"),
             QT_TRANSLATE_NOOP("action", "Insert note B"),
             ActionCategory::NoteInput
             ),
    UiAction("rest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Rest"),
             QT_TRANSLATE_NOOP("action", "Enter rest"),
             ActionCategory::NoteInput
             ),
    UiAction("rest-1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Whole rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Whole rest"),
             ActionCategory::NoteInput
             ),
    UiAction("rest-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Half rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Half rest"),
             ActionCategory::NoteInput
             ),
    UiAction("rest-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quarter rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Quarter rest"),
             ActionCategory::NoteInput
             ),
    UiAction("rest-8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Eighth rest"),
             QT_TRANSLATE_NOOP("action", "Note input: Eighth rest"),
             ActionCategory::NoteInput
             ),
    UiAction("fret-0",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 0 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 0 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-1",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 1 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 1 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-2",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 2 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 2 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-3",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 3 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 3 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-4",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 4 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 4 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-5",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 5 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 5 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-6",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 6 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 6 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-7",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 7 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 7 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-8",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 8 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 8 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-9",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 9 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 9 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-10",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 10 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 10 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-11",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 11 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 11 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-12",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 12 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 12 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-13",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 13 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 13 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("fret-14",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fret 14 (TAB)"),
             QT_TRANSLATE_NOOP("action", "Add fret 14 on current string (TAB only)"),
             ActionCategory::Tablature
             ),
    UiAction("add-8va",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ottava 8va alta"),
             QT_TRANSLATE_NOOP("action", "Add ottava 8va alta"),
             ActionCategory::Articulations
             ),
    UiAction("add-8vb",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Ottava 8va bassa"),
             QT_TRANSLATE_NOOP("action", "Add ottava 8va bassa"),
             ActionCategory::Articulations
             ),
    UiAction("add-hairpin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Crescendo"),
             QT_TRANSLATE_NOOP("action", "Add crescendo"),
             ActionCategory::Articulations
             ),
    UiAction("add-hairpin-reverse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Decrescendo"),
             QT_TRANSLATE_NOOP("action", "Add decrescendo"),
             ActionCategory::Articulations
             ),
    UiAction("add-noteline",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Note anchored line"),
             ActionCategory::Lines
             ),
    UiAction("chord-tie",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add tied note to chord"),
             ActionCategory::NoteInput
             ),
    UiAction("title-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Title"),
             QT_TRANSLATE_NOOP("action", "Add title text"),
             ActionCategory::Text
             ),
    UiAction("subtitle-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Subtitle"),
             QT_TRANSLATE_NOOP("action", "Add subtitle text"),
             ActionCategory::Text
             ),
    UiAction("composer-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Composer"),
             QT_TRANSLATE_NOOP("action", "Add composer text"),
             ActionCategory::Text
             ),
    UiAction("poet-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Lyricist"),
             QT_TRANSLATE_NOOP("action", "Add lyricist text"),
             ActionCategory::Text
             ),
    UiAction("part-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Part name"),
             QT_TRANSLATE_NOOP("action", "Add part name"),
             ActionCategory::Text
             ),
    UiAction("system-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "System text"),
             QT_TRANSLATE_NOOP("action", "Add system text"),
             ActionCategory::Text
             ),
    UiAction("staff-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Staff text"),
             QT_TRANSLATE_NOOP("action", "Add staff text"),
             ActionCategory::Text
             ),
    UiAction("expression-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Expression text"),
             QT_TRANSLATE_NOOP("action", "Add expression text"),
             ActionCategory::Text
             ),
    UiAction("rehearsalmark-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Rehearsal mark"),
             QT_TRANSLATE_NOOP("action", "Add rehearsal mark"),
             ActionCategory::Text
             ),
    UiAction("instrument-change-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Instrument change"),
             QT_TRANSLATE_NOOP("action", "Add instrument change"),
             ActionCategory::Text
             ),
    UiAction("fingering-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Fingering"),
             QT_TRANSLATE_NOOP("action", "Add fingering"),
             ActionCategory::Text
             ),
    UiAction("sticking-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sticking"),
             QT_TRANSLATE_NOOP("action", "Add sticking"),
             ActionCategory::Text
             ),
    UiAction("chord-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Chord symbol"),
             QT_TRANSLATE_NOOP("action", "Add chord symbol"),
             ActionCategory::Text
             ),
    UiAction("roman-numeral-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Roman numeral analysis"),
             QT_TRANSLATE_NOOP("action", "Add Roman numeral analysis"),
             ActionCategory::Text
             ),
    UiAction("nashville-number-text",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Nashville number"),
             QT_TRANSLATE_NOOP("action", "Add Nashville number"),
             ActionCategory::Text
             ),
    UiAction("lyrics",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Lyrics"),
             QT_TRANSLATE_NOOP("action", "Add lyrics"),
             ActionCategory::Text
             ),
    UiAction("figured-bass",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Figured bass"),
             QT_TRANSLATE_NOOP("action", "Add figured bass"),
             ActionCategory::Articulations
             ),
    UiAction("tempo",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Tempo marking"),
             QT_TRANSLATE_NOOP("action", "Add tempo marking"),
             ActionCategory::Articulations
             ),
    UiAction("duplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Duplet"),
             QT_TRANSLATE_NOOP("action", "Add duplet"),
             ActionCategory::NoteInput
             ),
    UiAction("triplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Triplet"),
             QT_TRANSLATE_NOOP("action", "Add triplet"),
             ActionCategory::NoteInput
             ),
    UiAction("quadruplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quadruplet"),
             QT_TRANSLATE_NOOP("action", "Add quadruplet"),
             ActionCategory::NoteInput
             ),
    UiAction("quintuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quintuplet"),
             QT_TRANSLATE_NOOP("action", "Add quintuplet"),
             ActionCategory::NoteInput
             ),
    UiAction("sextuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Sextuplet"),
             QT_TRANSLATE_NOOP("action", "Add sextuplet"),
             ActionCategory::NoteInput
             ),
    UiAction("septuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Septuplet"),
             QT_TRANSLATE_NOOP("action", "Add septuplet"),
             ActionCategory::NoteInput
             ),
    UiAction("octuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Octuplet"),
             QT_TRANSLATE_NOOP("action", "Add octuplet"),
             ActionCategory::NoteInput
             ),
    UiAction("nonuplet",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Nontuplet"),
             QT_TRANSLATE_NOOP("action", "Add nontuplet"),
             ActionCategory::NoteInput
             ),
    UiAction("tuplet-dialog",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Other…"),
             QT_TRANSLATE_NOOP("action", "Other tuplets"),
             ActionCategory::DialogsAndWindows
             ),
    UiAction("stretch-",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Decrease layout stretch"),
             QT_TRANSLATE_NOOP("action", "Decrease layout stretch factor of selected measures"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("stretch+",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Increase layout stretch"),
             QT_TRANSLATE_NOOP("action", "Increase layout stretch factor of selected measures"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("reset-stretch",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset layout stretch"),
             QT_TRANSLATE_NOOP("action", "Reset layout stretch factor of selected measures or entire score"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("reset-text-style-overrides",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset text style overrides"),
             QT_TRANSLATE_NOOP("action", "Reset all text style overrides to default"),
             ActionCategory::Text
             ),
    UiAction("reset-beammode",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset beams"),
             QT_TRANSLATE_NOOP("action", "Reset beams of selected measures"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("reset",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset shapes and positions"),
             QT_TRANSLATE_NOOP("action", "Reset shapes and positions of selected elements to their defaults"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("zoomin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Zoom in"),
             QT_TRANSLATE_NOOP("action", "Zoom in"),
             IconCode::Code::ZOOM_IN,
             ActionCategory::System
             ),
    UiAction("zoomout",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Zoom out"),
             QT_TRANSLATE_NOOP("action", "Zoom out"),
             IconCode::Code::ZOOM_OUT,
             ActionCategory::System
             ),
    UiAction("zoom100",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Zoom to 100%"),
             ActionCategory::System
             ),
    UiAction("get-location",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Accessibility: Get location"),
             ActionCategory::System
             ),
    UiAction("edit-element",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Edit element"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("select-prev-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to beginning of measure"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-next-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to end of measure"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-begin-line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to beginning of line"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-end-line",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to end of line"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-begin-score",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to beginning of score"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-end-score",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Select to end of score"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-staff-above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add staff above to selection"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("select-staff-below",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add staff below to selection"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("scr-prev",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Screen: Previous"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("scr-next",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Screen: Next"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("page-prev",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Previous"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("page-next",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Next"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("page-top",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Top of first"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("page-end",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Page: Bottom of last"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("help",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Context sensitive help"),
             ActionCategory::DialogsAndWindows
             ),
    UiAction("repeat-sel",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Repeat selection"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("lock",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle score lock"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("enh-both",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (both modes)"),
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (both modes)"),
             IconCode::Code::NONE,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("enh-current",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (current mode)"),
             QT_TRANSLATE_NOOP("action", "Change enharmonic spelling (current mode)"),
             IconCode::Code::NONE,
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("flip",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Flip direction"),
             QT_TRANSLATE_NOOP("action", "Flip direction"),
             IconCode::Code::NOTE_FLIP,
             ActionCategory::NoteInput
             ),
    UiAction(TOGGLE_CONCERT_PITCH_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Concert pitch"),
             QT_TRANSLATE_NOOP("action", "Toggle 'Concert pitch'"),
             IconCode::Code::TUNING_FORK, ActionCategory::Undefined,
             ActionCategory::LayoutAndFormatting,
             Checkable::Yes
             ),
    UiAction("print",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Print"),
             QT_TRANSLATE_NOOP("action", "Print score/part"),
             IconCode::Code::PRINT,
             ActionCategory::DialogsAndWindows
             ),
    UiAction("next-text-element",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Next text element"),
             QT_TRANSLATE_NOOP("action", "Move to text element on next note"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("prev-text-element",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Previous text element"),
             QT_TRANSLATE_NOOP("action", "Move to text element on previous note"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("next-beat-TEXT",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Next beat (Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Next beat (Chord symbol)"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("prev-beat-TEXT",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Previous beat (Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Previous beat (Chord symbol)"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("advance-longa",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Longa advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a longa (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-breve",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Breve advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a double whole note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-1",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Whole note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a whole note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-2",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Half note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a half note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-4",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Quarter note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a quarter note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-8",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "Eighth note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of an eighth note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-16",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "16th note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a 16th note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-32",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "32nd note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a 32nd note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("advance-64",
             mu::context::UiCtxNotationFocused,
             QT_TRANSLATE_NOOP("action", "64th note advance (F.B./Chord symbol)"),
             QT_TRANSLATE_NOOP("action", "Advance of a 64th note (Figured bass/Chord symbol only)"),
             ActionCategory::NoteInput
             ),
    UiAction("next-lyric-verse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next lyric verse"),
             QT_TRANSLATE_NOOP("action", "Move to lyric in the next verse"),
             ActionCategory::Articulations
             ),
    UiAction("prev-lyric-verse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Previous lyric verse"),
             QT_TRANSLATE_NOOP("action", "Move to lyric in the previous verse"),
             ActionCategory::Articulations
             ),
    UiAction("next-syllable",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Next syllable"),
             QT_TRANSLATE_NOOP("action", "Add hyphen and move to lyric on next note"),
             ActionCategory::NoteInput
             ),
    UiAction("add-melisma",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add melisma"),
             QT_TRANSLATE_NOOP("action", "Add melisma line and move to lyric on next note"),
             ActionCategory::Lines
             ),
    UiAction("add-lyric-verse",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add lyric verse"),
             QT_TRANSLATE_NOOP("action", "Adds a new verse and starts editing"),
             ActionCategory::Text
             ),
    UiAction("text-b",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Bold face"),
             ActionCategory::Text,
             Checkable::Yes
             ),
    UiAction("text-i",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Italic"),
             ActionCategory::Text,
             Checkable::Yes
             ),
    UiAction("text-u",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Underline"),
             ActionCategory::Text,
             Checkable::Yes
             ),
    UiAction("text-s",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Strike"),
             ActionCategory::Text,
             Checkable::Yes
             ),
    UiAction("pitch-up-diatonic",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch up"),
             ActionCategory::Articulations
             ),
    UiAction("pitch-down-diatonic",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch down"),
             ActionCategory::Articulations
             ),
    UiAction("top-staff",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Go to top staff"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("empty-trailing-measure",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Go to first empty trailing measure"),
             ActionCategory::SelectionAndNavigation
             ),
    UiAction("mirror-note",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Mirror notehead"),
             ActionCategory::NoteInput
             ),
    UiAction("add-trill",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle trill"),
             ActionCategory::NoteInput
             ),
    UiAction("add-up-bow",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle up bow"),
             ActionCategory::NoteInput
             ),
    UiAction("add-down-bow",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle down bow"),
             ActionCategory::NoteInput
             ),
    UiAction("reset-style",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Reset style"),
             QT_TRANSLATE_NOOP("action", "Reset all style values to default"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("clef-violin",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add treble clef"),
             ActionCategory::Articulations
             ),
    UiAction("clef-bass",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Add bass clef"),
             ActionCategory::Articulations
             ),
    UiAction("sharp2-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double ♯ (non-toggle)"),
             ActionCategory::NoteInput
             ),
    UiAction("sharp-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "♯ (non-toggle)"),
             ActionCategory::NoteInput
             ),
    UiAction("nat-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "♮ (non-toggle)"),
             ActionCategory::NoteInput
             ),
    UiAction("flat-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "♭ (non-toggle)"),
             ActionCategory::NoteInput
             ),
    UiAction("flat2-post",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Double ♭ (non-toggle)"),
             ActionCategory::NoteInput
             ),
    UiAction("transpose-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose up"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("transpose-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose down"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("pitch-up-diatonic-alterations",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch up (keep degree alterations)"),
             ActionCategory::Articulations
             ),
    UiAction("pitch-down-diatonic-alterations",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Diatonic pitch down (keep degree alterations)"),
             ActionCategory::Articulations
             ),
    UiAction("transpose-down",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose down"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("transpose-up",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Transpose up"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("full-measure-rest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Full measure rest"),
             ActionCategory::NoteInput
             ),
    UiAction("toggle-mmrest",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'Create multimeasure rest'"),
             ActionCategory::NoteInput
             ),
    UiAction("toggle-hide-empty",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'Hide empty staves'"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("set-visible",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Set visible"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("unset-visible",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Set invisible"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("toggle-autoplace",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'automatic placement' for selected elements"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("autoplace-enabled",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Toggle 'automatic placement' (whole score)"),
             ActionCategory::LayoutAndFormatting
             ),
    UiAction("string-above",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "String Above (TAB)"),
             ActionCategory::Tablature
             ),
    UiAction("string-below",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "String Below (TAB)"),
             ActionCategory::Tablature
             ),
    UiAction("pad-note-1-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Whole note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: whole note (TAB)"),
             IconCode::Code::NOTE_WHOLE,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-2-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Half note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: half note (TAB)"),
             IconCode::Code::NOTE_HALF,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-4-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Quarter note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: quarter note (TAB)"),
             IconCode::Code::NOTE_QUARTER,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-8-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "8th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 8th note (TAB)"),
             IconCode::Code::NOTE_8TH,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-16-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "16th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 16th note (TAB)"),
             IconCode::Code::NOTE_16TH,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-32-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "32nd note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 32nd note (TAB)"),
             IconCode::Code::NOTE_32ND,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-64-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "64th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 64th note (TAB)"),
             IconCode::Code::NOTE_64TH,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-128-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "128th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 128th note (TAB)"),
             IconCode::Code::NOTE_128TH,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-256-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "256th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 256th note (TAB)"),
             IconCode::Code::NOTE_256TH,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-512-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "512th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 512th note (TAB)"),
             IconCode::Code::NOTE_512TH,
             ActionCategory::Tablature
             ),
    UiAction("pad-note-1024-TAB",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "1024th note (TAB)"),
             QT_TRANSLATE_NOOP("action", "Note duration: 1024th note (TAB)"),
             IconCode::Code::NOTE_1024TH,
             ActionCategory::Tablature
             ),
    UiAction("notation-context-menu",
             mu::context::UiCtxNotationFocused,
             ActionCategory::DialogsAndWindows
             )
};

const UiActionList NotationUiActions::m_noteInputActions = {
    UiAction(NOTE_INPUT_ACTION_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Note input"),
             QT_TRANSLATE_NOOP("action", "Enter notes with a mouse or keyboard"),
             IconCode::Code::EDIT, ActionCategory::Undefined,

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
             QT_TRANSLATE_NOOP("action", "Show invisible"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction(SHOW_UNPRINTABLE_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show formatting"),
             QT_TRANSLATE_NOOP("action", "Show formatting"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction(SHOW_FRAMES_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show frames"),
             QT_TRANSLATE_NOOP("action", "Show frames"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction(SHOW_PAGEBORDERS_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show page margins"),
             QT_TRANSLATE_NOOP("action", "Show page margins"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction(SHOW_IRREGULAR_CODE,
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Mark irregular measures"),
             QT_TRANSLATE_NOOP("action", "Mark irregular measures"), ActionCategory::Undefined,
             Checkable::Yes
             )
};

const UiActionList NotationUiActions::m_engravingDebuggingActions = {
    UiAction("show-skylines",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show skylines"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction("show-segment-shapes",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show segment shapes"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction("show-bounding-rect",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show bounding rectangles"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction("show-system-bounding-rect",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show system bounding rectangles"), ActionCategory::Undefined,
             Checkable::Yes
             ),
    UiAction("show-corrupted-measures",
             mu::context::UiCtxNotationOpened,
             QT_TRANSLATE_NOOP("action", "Show corrupted measures"), ActionCategory::Undefined,
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
