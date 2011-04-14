
(define (stv mean conf) (cog-new-stv mean conf))

; Some data to populate the atomspace
(AssociativeLink (stv 1.0 1.0)
   (ConceptNode "want-this")
   (ConceptNode "valid")
)

(AssociativeLink (stv 1.0 1.0)
   (ConceptNode "want-this")
   (ConceptNode "one-high, 4-arity")
   (ConceptNode "mehh2")
   (ConceptNode "mehh3")
)

(AssociativeLink (stv 1.0 1.0)
   (ConceptNode "want-this")
   (MemberLink
      (WordInstanceNode "color")
      (WordInstanceNode "blue")
   )
)

(AssociativeLink (stv 1.0 1.0)
   (MemberLink
      (WordInstanceNode "color")
      (WordInstanceNode "blue")
   )
   (ConceptNode "want-this")
)

(AssociativeLink (stv 1.0 1.0)
   (ConceptNode "want-this")
   (InheritanceLink
      (WordInstanceNode "color")
      (WordInstanceNode "green")
   )
)

(AssociativeLink (stv 1.0 1.0)
   (ConceptNode "want-this")
   (MemberLink
      (WordInstanceNode "color")
      (WordInstanceNode "blue")
   )
   (ConceptNode "mehh2")
   (ConceptNode "mehh3")
)

; Match any arity-2 structure of the desired form.
(define (untyped-link-match)
   (BindLink
      (ListLink 
         (VariableNode "$var")
      )
      (ImplicationLink
         (AndLink
            (AssociativeLink (stv 1 0.99999988)
               (ConceptNode "want-this")
               (VariableNode "$var")
            )
         )
         (VariableNode "$var")
      )
   )
)

; Match arity-2 with the link having a type.
(define (typed-link-match)
   (BindLink
      (ListLink 
         (TypedVariableLink
            (VariableNode "$var")
            (VariableTypeNode "MemberLink")
         )
      )
      (ImplicationLink
         (AndLink
            (AssociativeLink (stv 1 0.99999988)
               (ConceptNode "want-this")
               (VariableNode "$var")
            )
         )
         (VariableNode "$var")
      )
   )
)


; Match any arity two structure
(define (untyped-any-match)
   (BindLink
      (ListLink 
         (VariableNode "$var-a")
         (VariableNode "$var-b")
      )
      (ImplicationLink
         (AndLink
            (AssociativeLink (stv 1 0.99999988)
               (VariableNode "$var-a")
               (VariableNode "$var-b")
            )
         )
         (ListLink
            (VariableNode "$var-a")
            (VariableNode "$var-b")
         )
      )
   )
)

