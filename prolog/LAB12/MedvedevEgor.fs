open System

let epsilon = 0.000001 


let rec dichotomy f a b =
    let mid = (a + b) / 2.0
    let fMid = f mid
    if Math.Abs(fMid : float) < epsilon then mid
    elif fMid * f a <= 0.0 then dichotomy f a mid
    else dichotomy f mid b

// Производные уравнений
let equation14_derivative x = (1.0 / (cos(x / 2.0) ** 2.0)) + (1.0 / (sin(x / 2.0) ** 2.0)) + 1.0
let equation15_derivative x = (1.0 / (1.0 + x)) - 1.0
let equation16_derivative x = (3.0 * cos(sqrt x)) / (2.0 * sqrt x) + 0.35


let rec newton f f' x0 =
    let nextX = x0 - f x0 / f' x0
    if Math.Abs(nextX - x0 : float) < epsilon then nextX
    else newton f f' nextX


let equation14 x = Math.Tan(x / 2.0) - 1.0 / Math.Tan(x / 2.0) + x
let equation15 x = 0.4 + Math.Atan(Math.Sqrt(x)) - x
let equation16 x = 3.0 * Math.Sin(Math.Sqrt(x)) + 0.35 * x - 3.8


let phi14 x = x - equation14 x / equation14_derivative x
let phi15 x = x - equation15 x / equation15_derivative x
let phi16 x = x - equation16 x / equation16_derivative x


let rec simpleIteration phi x0 =
    let nextX = phi x0
    if Math.Abs(nextX - x0 : float) < epsilon then nextX
    else simpleIteration phi nextX


let solveAndPrint equation derivative phi range =
    let a, b = range
    printfn "Dichotomy result: %A" (dichotomy equation a b)
    printfn "Newton result: %A" (newton equation derivative a) 
    printfn "Simple Iteration result: %A" (simpleIteration phi a) 
    printfn ""


printfn "Equation 14:"
printfn "Tan(x / 2) - 1 / Tan(x / 2) + x"
solveAndPrint equation14 equation14_derivative phi14 (1.0, 2.0)

printfn "Equation 15:"
printfn "0.4 + Atan(Sqrt(x)) - x"
solveAndPrint equation15 equation15_derivative phi15 (1.0, 2.0)

printfn "Equation 16:"
printfn "3.0 * Sin(Sqrt(x)) + 0.35 * x - 3.8"
solveAndPrint equation16 equation16_derivative phi16 (2.0, 3.0)


