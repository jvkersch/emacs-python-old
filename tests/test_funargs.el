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


(ert-deftest test-run-len ()
  :tags '(run-len)
  (should (equal (py_len_str "i am a string") 13)))
