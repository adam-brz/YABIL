import random
import string


def generate_op(op_len, op_positive=False):
    if op_len == 0:
        return "0"
    op = random.choice("123456789")
    sign = "" if op_positive else random.choice(["-", ""])
    return sign + op + "".join(random.choice(string.digits) for _ in range(op_len - 1))


def generate_check(
    op1_len, op2_len, operator, str_template, op1_positive=False, op2_positive=False
):
    op1 = generate_op(op1_len, op1_positive)
    op2 = generate_op(op2_len, op2_positive)
    ret = int(eval(f"{op1} {'//' if operator == '/' else operator} {op2}"))
    print(
        str_template.format(op1, operator, op2, ret)
        # + f' << ", ret != " << (bigint::BigInt("{op1}") {operator} bigint::BigInt("{op2}")).to_str()'
        + ";"
    )


def generate_arith_check(
    op1_len, op2_len, operator, op1_positive=False, op2_positive=False
):
    generate_check(
        op1_len,
        op2_len,
        operator,
        "EXPECT_TRUE(({}_bi {} {}_bi) == {}_bi)",
        op1_positive,
        op2_positive,
    )


def generate_rel_check(
    op1_len, op2_len, operator, op1_positive=False, op2_positive=False
):
    generate_check(
        op1_len,
        op2_len,
        operator,
        "EXPECT_TRUE(({}_bi {} {}_bi) == {})",
        op1_positive,
        op2_positive,
    )


def generate_shift_check(
    op1_len, op2_len, operator, op1_positive=True, op2_positive=True
):
    generate_check(
        op1_len,
        op2_len,
        operator,
        "EXPECT_TRUE(({}_bi {} shift_v<{}>) == {}_bi)",
        op1_positive,
        op2_positive,
    )


arith_operators = ["+", "-", "*", "/", "%"]
rel_operators = ["==", ">", ">=", "<", "<="]

for op_len in range(2, 48):
    for op in ["+", "-"]:
        generate_arith_check(op_len, op_len, op)

for op_len in range(2, 32):
    for op in ["/", "%"]:
        generate_arith_check(
            op_len, random.randint(1, op_len), op, op1_positive=True, op2_positive=True
        )

for op_len in range(2, 20):
    for op in ["*"]:
        generate_arith_check(op_len, op_len, op)

for op_len in range(2, 48):
    for op in rel_operators:
        generate_rel_check(random.randint(1, op_len), random.randint(1, op_len), op)

for op_len in range(2, 10):
    for op in ["<<", ">>"]:
        for shift_val in [1, 2]:
            generate_shift_check(
                op_len, shift_val, op, op1_positive=True, op2_positive=True
            )
