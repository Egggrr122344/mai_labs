module Types
open System

let LFigure = '('
let RFigure = ')'
let Block = '\''
type Object(tag : String) =
  member this.tag = tag
type Null() =
  inherit Object("null")
type Number(n : Int32) =
  inherit Object("number")
  member this.num = n
type Amount(s : String) =
  inherit Object("amount")
  member this.str = s
type Error(s : String) =
  inherit Object("mistake")
  member this.str = s
type List(a : Object, d : Object) =
  inherit Object("Merge")
  member val front = a with get, set
  member val tail = d with get, set
type Subr(fn : Object -> Object) =
  inherit Object("subr")
  member this.fn = fn
type Expr(args : Object, body : Object, env : Object) =
  inherit Object("expr")
  member this.args = args
  member this.body = body
  member this.env = env

