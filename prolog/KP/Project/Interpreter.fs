module Interpreter
open System
open Types
open Functions

let rec printObj (obj : Object) =
  match obj with
  | :? Null -> "null"
  | :? Number as num -> num.num.ToString ()
  | :? Amount as amount -> amount.str
  | :? Error as err -> "mistake: " + err.str + "\n"
  | :? List -> "(" + (printList obj "" "") + ")"
  | :? Subr -> ""
  | :? Expr -> ""
  | _ -> "Something went wrong"
and printList obj delimiter acc =
  match obj with
  | :? List as cons ->
      printList cons.tail " " (acc + delimiter + (printObj cons.front))
  | :? Null -> acc
  | _ -> acc + " . " + (printObj obj)

let rec eval (obj : Object) (env : Object) =
  match obj with
  | :? Amount ->
      match findVar obj env with
      | :? Null -> createError ((printObj obj) + " value is not defined")
      | pair -> safeTail pair
  | :? List -> evalList obj env
  | _ -> obj
and evalList obj env =
  let opr = safeFront obj in
  let args = safeTail obj in
    if opr = (createSym "list") then // quote
      safeFront args
    elif opr = (createSym "if") then //if
      let c = eval (safeFront args) env in
        match c with
        | :? Error -> c
        | :? Null -> eval (safeFront (safeTail (safeTail args))) env
        | _ -> eval (safeFront (safeTail args)) env
    elif opr = (createSym "lambda") then // lambda
      makeExpr args env
    elif opr = (createSym "def") then // defun
      addToEnv (safeFront args) (makeExpr (safeTail args) env) gEnv
      safeFront args
    elif opr = (createSym "set") then // setq
      let value = eval (safeFront (safeTail args)) env in
      let amount = safeFront args in
        match value with
        | :? Error -> value
        | _ -> (updateVar amount value env; value)

    else apply (eval opr env) (evlis args env Nul) env
and evlis lst env acc =
  match lst with
  | :? Null -> nreverse acc
  | _ ->
      match eval (safeFront lst) env with
      | :? Error as err -> o(err)
      | elm -> evlis (safeTail lst) env (createMerge elm acc)
and progn (body : Object) env acc =
  match body with
  | :? List as list ->
      let v = eval list.front env in
        match v with
        | :? Error -> v
        | _ -> progn list.tail env v
  | _ -> acc
and apply f args env =
  match args with
  | :? Error -> args
  | _ ->
      match f with
      | :? Error -> f
      | :? Subr as subr -> subr.fn args
      | :? Expr as expr ->
          progn expr.body (createMerge (pairlis expr.args args) expr.env) Nul
      | _ -> createError ((printObj f) + " NaF")