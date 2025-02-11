module Parser
open Types
open Functions

let readAtom str =
  match position isDelimiter str with
  | Some n -> (makeNumberOrSymbo (str.Substring (0, n)), str.Substring n)
  | None -> (makeNumberOrSymbo str, "")

let lookAhead str =
  let str1 = skipSpaces str in
  let c = if str1 = "" then '_' else str.[0] in
  let rest = if str1 = "" then ""
             else str.Substring 1 in
    (str1, c, rest)


let rec read str =
  let (str1, c, rest) = lookAhead str in
    if str1 = "" then (createError "input is empty", "")
    elif c = RFigure then (createError ("invalid arguments: " + str), "")
    elif c = LFigure then readList rest Nul
    elif c = Block then readBlock rest
    else readAtom str1
and readBlock str =
  let (elm, next) = read str in
    (createMerge (createSym "list") (createMerge elm Nul), next)
and readList str acc =
  let (str1, c, rest) = lookAhead str in
    if str1 = "" then (createError "unfinished brackets", "")
    elif c = RFigure then (nreverse acc, rest)
    else
      let (obj, next) = read str1 in
      match obj with
      | :? Error -> (obj, next)
      | _ -> readList next (createMerge obj acc)