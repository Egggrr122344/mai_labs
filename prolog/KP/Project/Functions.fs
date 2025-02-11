module Functions
open System
open Types



let o obj = obj :> Object
let Nul = o(Null ())

let safeFront (obj : Object) =
  match obj with
  | :? List as c -> c.front
  | _ -> Nul

let safeTail (obj : Object) =
  match obj with
  | :? List as c -> c.tail
  | _ -> Nul

let createNumber n = o(Number n)
let createError s = o(Error s)
let createMerge a d = o(List (a, d))
let makeSubr fn = o(Subr fn)
let makeExpr args env = o(Expr (safeFront args, safeTail args, env))

let symTable = ref (Map.ofList [("bull", Nul)])
let createSym str =
  if Map.containsKey str !symTable then
    Map.find str !symTable
  else
    let amount = o(Amount str) in
      symTable := Map.add str amount !symTable;
      amount

let rec nreconc (lst : Object) (tail : Object) =
  match lst with
  | :? List as l ->
      let tmp = l.tail in
        l.tail <- tail;
        nreconc tmp lst
  | _ -> tail
let nreverse lst = nreconc lst Nul

let pairlis (lst1 : Object) (lst2 : Object) =
  let rec doit (lst1 : Object) (lst2 : Object) acc =
    match lst1 with
    | :? List as list1 ->
        match lst2 with
        | :? List as list2 ->
            doit list1.tail list2.tail
                 (createMerge (createMerge list1.front list2.front) acc)
        | _ -> nreverse acc
    | _ -> nreverse acc
  doit lst1 lst2 Nul

let isSpace c =
  c = '\t' || c = '\r' || c = '\n' || c = ' '

let isDelimiter c =
  c = LFigure || c = RFigure || c = Block || isSpace c

let skipSpaces str =
  let rec doit i =
    if i = String.length str then ""
    elif isSpace str.[i] then doit (i + 1)
    else str.Substring(i)
  doit 0

let makeNumberOrSymbo str =
  try createNumber (Int32.Parse str)
  with
   | :? FormatException -> createSym str

let position f str =
  let rec doit i =
    if i = String.length str then None
    elif f str.[i] then Some i
    else doit (i + 1)
  doit 0


let rec findVarInFrame (amount : Object) (alist : Object) =
  match safeFront (safeFront alist) with
  | :? Amount as s-> if o(s) = amount then safeFront alist
                             else findVarInFrame amount (safeTail alist)
  | _ -> Nul
let rec findVar (amount : Object) (env : Object) =
  match env with
  | :? List as list ->
      match findVarInFrame amount list.front with
      | :? Null -> findVar amount list.tail
      | pair -> pair
  | _ -> Nul

let gEnv = createMerge Nul Nul

let addToEnv (amount : Object) (value : Object) (env : Object) =
  match env with
  | :? List as list -> list.front <- createMerge (createMerge amount value) list.front
  | _ -> ()

let updateVar amount value env =
  match findVar amount env with
  | :? List as list -> list.tail <- value
  | _ -> addToEnv amount value gEnv

  
let first (x, y) = x
