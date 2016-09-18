; Needed to find the pymacs SO
(add-to-list 'load-path "..")

; Load library
(require 'pymacs)

; Import module
(load-python-module "funargs")

(ert-deftest test-run-add ()
  :tags '(run-add)
  (should (equal (py_add 2 3) 5)))
