; Needed to find the pymacs SO
(add-to-list 'load-path ".")

; Load library
(require 'pymacs)

; Import module
(load-python-module "returntype")

; Tests
(ert-deftest test-run-int ()
  :tags '(run-int)
  (should (equal (method_that_returns_an_int) 42)))

(ert-deftest test-run-string ()
  :tags '(run-string)
  (should (equal (method_that_returns_a_string) "str")))

(ert-deftest test-run-float ()
  :tags '(run-float)
  (should (equal (method_that_returns_a_float) 3.1415)))

(ert-deftest test-run-object ()
  :tags '(run-object)
  (should (equal (type-of (method_that_returns_an_object)) 'user-ptr)))
