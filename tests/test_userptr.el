; Needed to find the pymacs SO
(add-to-list 'load-path "../src")

; Load library
(require 'pymacs)

; Import module
(load-python-module "userptr")


(defun instantiate-object-and-run (n)
  "Instantiate an opaque MyObject and increase the counter n times."
  (setq count 0)
  (setq obj (make_object))
  (while (< count n)
    (increase_counter obj)
    (setq count (1+ count)))
  (get_value obj))


(ert-deftest test-run-increase-counter ()
  (setq myobj (make_object))
  (should (equal (instantiate-object-and-run 20) 20)))
