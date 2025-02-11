open System
open Types
open Functions
open Parser
open Interpreter



let Front args = safeFront (safeFront args)
let Tail args = safeTail (safeFront args)
let Merge args = createMerge (safeFront args) (safeFront (safeTail args))

let subrEq args =
  let x = safeFront args in
  let y = safeFront (safeTail args) in
  match x with
  | :? Number as n1 ->
      match y with
      | :? Number as n2 ->
          if n1.num = n2.num then createSym "t"
          else Nul
      | _ -> Nul
  | _ -> if x = y then createSym "t"
         else Nul

let sAtom args =
  match safeFront args with
  | :? List -> Nul
  | _ -> createSym "t"

let sNumberp args =
  match safeFront args with
  | :? Number -> createSym "t"
  | _ -> Nul

let sSymbolp args =
  match safeFront args with
  | :? Amount -> createSym "t"
  | _ -> Nul

let subrAddOrMul f initVal =
  let rec doit (args : Object) acc =
    match args with
    | :? List as list ->
        match list.front with
        | :? Number as n -> doit list.tail (f (acc, n.num))
        | _ -> createError "incorrect type"
    | _ -> createNumber acc
  in fun args -> doit args initVal
let subrAdd = subrAddOrMul (fun (x, y) -> x + y) 0
let subrMul = subrAddOrMul (fun (x, y) -> x * y) 1

let subrAllF f =
  fun (args : Object) ->
    match safeFront args with
    | :? Number as n1 ->
        match safeFront (safeTail args) with
        | :? Number as n2 -> createNumber (f (n1.num, n2.num))
        | _ -> createError "incorrect type"
    | _ -> createError "incorrect type"
let subrSub : Object -> Object = subrAllF (fun (x, y) -> x - y)
let subrDiv : Object -> Object = subrAllF (fun (x, y) -> x / y)
let subrMod : Object -> Object = subrAllF (fun (x, y) -> x % y)


let rec reply () =
  Console.Write "-- "
  match Console.ReadLine () with
  | null -> ()
  | line ->
      Console.WriteLine (printObj (eval (first (read line)) gEnv))
      reply ()

let () = 
  addToEnv (createSym "head") (makeSubr Front) gEnv 
  addToEnv (createSym "tail") (makeSubr Tail) gEnv 
  addToEnv (createSym "merge") (makeSubr Merge) gEnv 
  addToEnv (createSym "eq") (makeSubr subrEq) gEnv
  addToEnv (createSym "atom") (makeSubr sAtom) gEnv
  addToEnv (createSym "numberp") (makeSubr sNumberp) gEnv
  addToEnv (createSym "symbolp") (makeSubr sSymbolp) gEnv
  addToEnv (createSym "+") (makeSubr subrAdd) gEnv
  addToEnv (createSym "*") (makeSubr subrMul) gEnv
  addToEnv (createSym "-") (makeSubr subrSub) gEnv
  addToEnv (createSym "/") (makeSubr subrDiv) gEnv
  addToEnv (createSym "mod") (makeSubr subrMod) gEnv
  addToEnv (createSym "t") (createSym "t") gEnv
  reply ()