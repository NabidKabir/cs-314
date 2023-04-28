
; *********************************************
; *  314 Principles of Programming Languages  *
; *  Spring 2023                              *
; *  Student Version                          *
; *********************************************

;; contains "ctv", "A", and "reduce" definitions
(load "include.ss")

;; contains simple dictionary definition
(load "test-dictionary.ss")

;; -----------------------------------------------------
;; HELPER FUNCTIONS

;; *** CODE FOR ANY HELPER FUNCTION GOES HERE ***


(define contains
  (lambda (elem l)
    (cond
      ((null? l) #f)
      ((= elem (car l)) #t)
      (else (contains elem (cdr l))))))

(define list-contains
  (lambda (l1 l2)
    (cond
      ((null? l1) #t)
      ((contains (car l1) l2) (list-contains (cdr l1) l2))
      (else #f))))

;;

(define gen-bitvector-entry
  (lambda (hashfunctionlist entry)
    (cond
      ((null? hashfunctionlist) '())
      (else (append (list ((car hashfunctionlist) entry)) (gen-bitvector-entry (cdr hashfunctionlist) entry))))))

(define gen-bitvector
  (lambda (hashfunctionlist d)
    (cond
      ((null? d) d)
      (else (append (gen-bitvector-entry hashfunctionlist (car d)) (gen-bitvector hashfunctionlist (cdr d)))))))
;; -----------------------------------------------------
;; KEY FUNCTION

(define key
  (lambda (w)
    (cond
      ((null? w) 0)
      (else (reduce (lambda (x y) (+ (* 29 y) (ctv x))) w 5077)))
))

;; -----------------------------------------------------
;; EXAMPLE KEY VALUES
;;   (key '(h e l l o))       = 104146015601
;;   (key '(m a y))           = 123844020
;;   (key '(t r e e f r o g)) = 2539881083658035

;; -----------------------------------------------------
;; HASH FUNCTION GENERATORS

;; value of parameter "size" should be a prime number
(define gen-hash-division-method
  (lambda (size) ;; range of values: 0..size-1
     (lambda (k) (modulo (key k) size)) ;; *** FUNCTION BODY IS MISSING ***
))

;; Note: hash functions may return integer values in "real"
;;       format, e.g., 17.0 for 17

(define gen-hash-multiplication-method
  (lambda (size) ;; range of values: 0..size-1
      (lambda (k)
      (let ((kA (* (key k) A)))
      (let ((fraction (- kA (floor kA))))
          (floor (* size fraction))))) ;; *** FUNCTION BODY IS MISSING ***
))


;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTIONS AND HASH FUNCTION LISTS

(define hash-1 (gen-hash-division-method 70111))
(define hash-2 (gen-hash-division-method 89997))
(define hash-3 (gen-hash-multiplication-method 7221))
(define hash-4 (gen-hash-multiplication-method 900))

(define hashfl-1 (list hash-1 hash-2 hash-3 hash-4))
(define hashfl-2 (list hash-1 hash-3))
(define hashfl-3 (list hash-2 hash-3))

;; -----------------------------------------------------
;; EXAMPLE HASH VALUES
;;   to test your hash function implementation
;;
;;  (hash-1 '(h e l l o))       ==> 51317
;;  (hash-1 '(m a y))           ==> 27994
;;  (hash-1 '(t r e e f r o g)) ==> 33645
;;
;;  (hash-2 '(h e l l o))       ==> 47249
;;  (hash-2 '(m a y))           ==> 8148
;;  (hash-2 '(t r e e f r o g)) ==> 53006
;;
;;  (hash-3 '(h e l l o))       ==> 4322.0
;;  (hash-3 '(m a y))           ==> 3288.0
;;  (hash-3 '(t r e e f r o g)) ==> 0.0
;;
;;  (hash-4 '(h e l l o))       ==> 538.0
;;  (hash-4 '(m a y))           ==> 409.0
;;  (hash-4 '(t r e e f r o g)) ==> 0.0


;; -----------------------------------------------------
;; SPELL CHECKER GENERATOR

(define gen-checker
  (lambda (hashfunctionlist dict)
      (let ((bitvector (gen-bitvector hashfunctionlist dict)))
      (lambda (w)
        (let ((w-vector (gen-bitvector hashfunctionlist (list w))))
          (list-contains w-vector bitvector)))) ;; *** FUNCTION BODY IS MISSING ***
))


;; -----------------------------------------------------
;; EXAMPLE SPELL CHECKERS

(define checker-1 (gen-checker hashfl-1 dictionary))
(define checker-2 (gen-checker hashfl-2 dictionary))
(define checker-3 (gen-checker hashfl-3 dictionary))

;; EXAMPLE APPLICATIONS OF A SPELL CHECKER
;;
;;  (checker-1 '(a r g g g g)) ==> #f
;;  (checker-2 '(h e l l o)) ==> #t
;;  (checker-2 '(a r g g g g)) ==> #f  

