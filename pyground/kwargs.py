def print_keyword_args(**kwargs):
     # kwargs is a dict of the keyword args passed to the function
    for key, value in kwargs.iteritems():
        print "%s = %s" % (key, value)


if __name__ == '__main__':
	print_keyword_args(first_name="John", last_name="Doe")
	first_name = John
	last_name = Doe
