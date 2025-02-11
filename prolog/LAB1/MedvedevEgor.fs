open System

let epsilon = 0.000001

let f x = (2.0 * x - 3.0) / Math.Pow(x - 1.0, 2.0)

let taylorNaive x n =
    let rec loop i acc =
        if i > n then acc, i
        else
            let term = -(float(i + 3)) * Math.Pow(x, float(i))
            if Math.Abs(term) < epsilon then acc + term, i
            else loop (i + 1) (acc + term)
    loop 0 0.0

let taylorSmart x =
    let rec smartLoop term n sum =
        if Math.Abs(term:float) < epsilon then sum, n
        else
            let nextTerm = -(float(n + 4)) * Math.Pow(x, float(n + 1))
            smartLoop nextTerm (n + 1) (sum + nextTerm)
    smartLoop (-3.0) 0 (-3.0)

let printResults a b step n =
    printfn "x\tTaylor series (dumb)\t# iterations\tTaylor series (smart)\t# iterations\tValue of a function"
    let rec loop x =
        if x <= b + step / 2.0 then
            let naiveValue, naiveIterations = taylorNaive x n
            let smartValue, smartIterations = taylorSmart x
            let builtin = f x
            printfn "%A\t%A\t\t%A\t\t%A\t\t\t%A\t\t%A" x naiveValue naiveIterations smartValue smartIterations builtin
            loop (x + step)
    loop a

let a = 0.1
let b = 0.6
let step = 0.02
let n = 20

printResults a b step n
