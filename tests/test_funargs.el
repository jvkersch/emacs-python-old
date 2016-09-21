; Needed to find the pymacs SO
(add-to-list 'load-path "..")

; Load library
(require 'pymacs)

; Import module
(load-python-module "funargs")

(ert-deftest test-run-add-int ()
  :tags '(run-add-int)
  (should (equal (py_add_int 2 3) 5)))


(ert-deftest test-run-add-float ()
  :tags '(run-add-float)
  (should (equal (py_add_float 2.0 3.0) 5.0)))


(ert-deftest test-run-len-str ()
  :tags '(run-len-str)
  (should (equal (py_len "i am a string") 13)))


(ert-deftest test-run-len-list ()
  :tags '(run-len-list)
  (should (equal (py_len (vector 1 2 3 4)) 4)))


(ert-deftest test-run-nested-argument ()
  :tags '(run-nested)
  (should (equal (py_nested_len
                  [["this" "is"] ["a" "nested" "list of strings"]])
                 28)))
