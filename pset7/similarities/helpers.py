from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    # TODO
    spl_a = a.split('\n')
    spl_b = b.split('\n')
    diff = (list(set(spl_a) - set(spl_b)))
    return (list(set(spl_a) - set(diff)))


def sentences(a, b):
    """Return sentences in both a and b"""
    # TODO
    sent_a = sent_tokenize(a, language='english')
    sent_b = sent_tokenize(b, language='english')
    diff = (list(set(sent_a) - set(sent_b)))
    return (list(set(sent_a) - set(diff)))


def to_substrings(str, n):
    ret_list = list()
    str_len = len(str)
    first_iteration = True
    start = 0
    while True:
        if first_iteration == True:
            first_iteration = False
        else:
            start += 1
        subst_stop = start + n
        if subst_stop > str_len:
            break
        ret_list.append(str[start:subst_stop])
    return ret_list


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    # TODO
    subst_a = to_substrings(a, n)
    subst_b = to_substrings(b, n)
    diff = (list(set(subst_a) - set(subst_b)))
    return (list(set(subst_a) - set(diff)))