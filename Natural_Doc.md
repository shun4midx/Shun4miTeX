# Shun4miTeX Natural Math Syntax

Natural math syntax is an easier way to type mathematics without knowing LaTeX. Shun4miTeX automatically converts the syntax below into proper LaTeX before rendering. Instead of relying on `{}` like in LaTeX, natural math syntax relies on spacing and `()` to determine how inputs are parsed.

---

# Inline vs Display Math

When using the command `/math`, wrap math with `@@...@@` for inline math or `@@@...@@@` for display math, like below.

```text
This is @@pi r^2@@.
```

```text
@@@
sum(i = 1, n, i^2)
@@@
```

---

# Function Syntax

Most special functions follow the form

```text
function(arg1, arg2, ...)
```

For example,

```text
sqrt(x)

frac(a, b)

root(5, x)

sum(i = 1, n, i^2)
```

Arguments are separated by commas. Parentheses are also used to group expressions, for example,

```text
e^(i pi) + 1 = 0
```

Unlike LaTeX, braces `{}` are generally unnecessary.

---

# Common examples

```text
x^2
e^(i pi)

frac(a, b)

sqrt(x)

root(3, x)

int(0, 1, f(x), x)

sum(i = 1, n, i)

lim(x, 0, f(x))

pmatrix(1, 2; 3, 4)

RR
alpha
∞
≤
```

---

# Symbols and Operators

```text
+  -  *  /  =
```

```text
<=   >=   !=
->   <-   =>
```

```text
cup
cap
subset
subseteq
in
notin
```

Examples:

```text
A subseteq B
```

```text
x -> x^2
```

---

# Exponents and Subscripts

```text
x^2
```

```text
e^(i pi)
```

Unicode exponents and subscripts are also supported.

```text
x²
a₁
```

---

# Greek Letters

Either spell them out:

```text
alpha beta gamma delta epsilon zeta eta theta
iota kappa lambda mu nu xi omicron pi rho
sigma tau upsilon phi chi psi omega

Gamma Delta Theta Lambda Xi Pi Sigma
Upsilon Phi Psi Omega
```

or use Unicode directly:

```text
α β Γ θ π ...
```

---

# Common Sets

```text
RR
ZZ
QQ
NN
CC
PP
AA
FF
HH
```

or

```text
ℝ ℤ ℚ ℕ ℂ ℙ 𝔸 𝔽 ℍ
```

---

# Fractions

```text
frac(a, b)
```

```text
frac(x + 1, y - 1)
```

---

# Roots

```text
sqrt(x)

cbrt(x)

root(5, x)
```

---

# Elementary Functions

The following functions are recognized automatically.

```text
sin(x)
cos(x)
tan(x)

sec(x)
csc(x)
cot(x)

arcsin(x)
arccos(x)
arctan(x)

sinh(x)
cosh(x)
tanh(x)

log(x)
ln(x)
exp(x)
```

Linear algebra and number theory operators are also supported.

```text
det(A)

rank(A)

ker(T)

dim(V)

gcd(a, b)

lcm(a, b)

max(a, b)

min(a, b)
```

These render using the corresponding LaTeX operator names.

---

# Absolute Value and Norm

```text
abs(x)

norm(v)
```

---

# Calculus

Integrals

```text
int(0, 1, x^2, x)

int(-infinity, infinity, e^(-x^2), x)
```

Derivatives

```text
deriv(y, x)

ddx(f, x)

pderiv(f, x)

pddx(f, x)
```

---

# Summation, Product and Limits

```text
sum(i = 1, n, i)

prod(i = 1, n, i)

lim(x, infinity, f(x))
```

---

# Matrices

```text
pmatrix(
1, 2;
3, 4
)
```

Other matrix environments:

```text
matrix(...)
bmatrix(...)
vmatrix(...)
```

Rows are separated by semicolons (`;`).

Columns are separated by commas (`,`).

---

# Miscellaneous

```text
boxed(x)

floor(x)

ceil(x)

angle ABC

triangle ABC

deg(45)
```

---

# Unicode Symbols

The following Unicode symbols may be typed directly.

```text
∞
≤
≥
≠
≈
→
←
↔
∈
∉
⊂
⊆
∪
∩
∅
×
·
±
∠
°
△
```

---

# Text Mode and Boxes

Normal text may be inserted inside mathematics using

```text
text(...)
```

For example,

```text
boxed(text(Hello!))
```

We may also box our answers, like so:

```text
boxed(frac(1, 2))
```

---

# Aliases

Many common names are accepted for convenience.

Infinity:

```text
infty
infinity
infin
oo
```

Sets:

```text
real
reals

integer
integers

natural
naturals

rational
rationals

complex
```

Empty set:

```text
empty
emptyset
nothing
```

Other aliases include

```text
forall
exists
grad
nabla
mod
cong
sim
```

---

# Mixing with LaTeX

Natural math does **not** replace LaTeX, so you may freely **mix both**!

For example,

```text
@@\frac{1}{2} + sqrt(x)@@
```

is perfectly valid.

---

# Notes
 - Natural Math is designed to make common mathematical notation easier to type.
 - If something is not currently supported, you can always write the corresponding LaTeX directly.
 - More syntax will be added in future releases.