#   Author:             Tom Goncin
#   File Description:   Use to aid the Python files within /Screen to run the UI with C++ code.
#   Date:               August 27th, 2026


def address(*args, **kwargs):
    return ""

def ADDRESSZ(*args, **kwargs):
    return ""

def ROUNDECIM(value, decimals=0):
    try:
        return round(float(value), decimals)
    except (ValueError, TypeError):
        return value