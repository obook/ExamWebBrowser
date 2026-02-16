<?php
// This file is part of Moodle - http://moodle.org/
//
// Moodle is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Moodle is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Moodle.  If not, see <http://www.gnu.org/licenses/>.

/**
 * Access rule for ExamWebBrowser.
 *
 * @package    quizaccess_ewb
 * @copyright  2024 obooklage
 * @license    http://www.gnu.org/copyleft/gpl.html GNU GPL v3 or later
 */

defined('MOODLE_INTERNAL') || die();

require_once($CFG->dirroot . '/mod/quiz/accessrule/accessrulebase.php');

/**
 * Quiz access rule that requires ExamWebBrowser.
 */
class quizaccess_ewb extends quiz_access_rule_base {

    /**
     * Check if the user is using ExamWebBrowser.
     *
     * @return bool
     */
    private function is_using_ewb() {
        $useragent = isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : '';
        return strpos($useragent, 'ExamWebBrowser') !== false;
    }

    /**
     * Whether the user should be blocked from starting a new attempt or continuing
     * an attempt now.
     *
     * @return string|false false if access should be allowed, a message explaining the
     *      reason if access should be prevented.
     */
    public function prevent_access() {
        if (has_capability('quizaccess/ewb:exemptfromcheck', $this->quizobj->get_context())) {
            return false;
        }

        if ($this->is_using_ewb()) {
            return false;
        }

        return get_string('ewbrequired', 'quizaccess_ewb');
    }

    /**
     * Information about this access rule to display to the student.
     *
     * @return string HTML fragment.
     */
    public function description() {
        return get_string('quizrequiresewb', 'quizaccess_ewb');
    }

    /**
     * Whether a preflight check is required.
     *
     * @param mixed $attemptid
     * @return bool
     */
    public function is_preflight_check_required($attemptid) {
        return false;
    }

    /**
     * Add settings form fields for this access rule.
     *
     * @param mod_quiz_mod_form $quizform
     * @param MoodleQuickForm $mform
     */
    public static function add_settings_form_fields(
            mod_quiz_mod_form $quizform, MoodleQuickForm $mform) {

        $mform->addElement('select', 'ewbrequired',
                get_string('requireewb', 'quizaccess_ewb'),
                [
                    0 => get_string('no'),
                    1 => get_string('yes'),
                ]);
        $mform->addHelpButton('ewbrequired', 'requireewb', 'quizaccess_ewb');
        $mform->setDefault('ewbrequired', 0);
    }

    /**
     * Save the settings for this access rule.
     *
     * @param stdClass $quiz the data from the quiz form.
     */
    public static function save_settings($quiz) {
        global $DB;

        if (empty($quiz->ewbrequired)) {
            $DB->delete_records('quizaccess_ewb', ['quizid' => $quiz->id]);
        } else {
            $record = $DB->get_record('quizaccess_ewb', ['quizid' => $quiz->id]);
            if ($record) {
                $record->ewbrequired = 1;
                $DB->update_record('quizaccess_ewb', $record);
            } else {
                $record = new stdClass();
                $record->quizid = $quiz->id;
                $record->ewbrequired = 1;
                $DB->insert_record('quizaccess_ewb', $record);
            }
        }
    }

    /**
     * Delete any rule-specific settings when the quiz is deleted.
     *
     * @param stdClass $quiz the data from the database.
     */
    public static function delete_settings($quiz) {
        global $DB;
        $DB->delete_records('quizaccess_ewb', ['quizid' => $quiz->id]);
    }

    /**
     * Get the settings for the quiz.
     *
     * @param int $quizid
     * @return stdClass|false
     */
    public static function get_settings_sql($quizid) {
        return [
            'ewb.ewbrequired',
            'LEFT JOIN {quizaccess_ewb} ewb ON ewb.quizid = quiz.id',
            [],
        ];
    }

    /**
     * Factory method — return an instance of this rule if it is required for the given quiz.
     *
     * @param \mod_quiz\quiz_settings $quizobj
     * @param int $timenow
     * @param bool $canignoretimelimits
     * @return self|null
     */
    public static function make(quiz $quizobj, $timenow, $canignoretimelimits) {
        if (!empty($quizobj->get_quiz()->ewbrequired)) {
            return new self($quizobj, $timenow);
        }
        return null;
    }
}
