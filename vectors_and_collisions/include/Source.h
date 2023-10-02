void tests() {

	// Line-line tests
	// Intersecting
	Line a{ 0, 2, 2, 8, 8 };
	Line b{ 0, 5, 1, 1, 5 };
	std::cout << a.to_string() << " and " << b.to_string() 
		<< " collision: " << line_line_intersect(a, b) << std::endl;

	//Non-Intersecting 
	Line c{ 0, 1, 2, 1, 5 };
	Line d{ 0, 3, 1, 3, 4 };
	std::cout << c.to_string() << " and " << d.to_string()
		<< " collision: " << line_line_intersect(c, d) << std::endl;


	// Line-box tests
	// Intersecting
	Box b1{ 0, 5, 5, 6, 6 };
	std::cout << a.to_string() << " and " << b1.to_string()
		<< " collision: " << line_box_intersect(a, b1) << std::endl;

	//Non-Intersecting 
	Box b2{ 0, 4, 4, 2, 2 };
	std::cout << c.to_string() << " and " << b2.to_string()
		<< " collision: " << line_box_intersect(c, b2) << std::endl;


	// Line-circle tests
	// Intersecting 
	Circle ccc1 {0, 7.770053, 1.284811, 0.603166};
    Line lll1 {0, 7.689738, 1.41839, 8.875329, 7.659048};
	std::cout << lll1.to_string() << " and " << ccc1.to_string()
		<< " collision: " << line_circle_intersect(lll1, ccc1) << std::endl;

	// Non-Intersecting 
	Circle c2{ 0, 5, 5, 4 };
	std::cout << c.to_string() << " and " << c2.to_string()
		<< " collision: " << line_circle_intersect(c, c2) << std::endl;


	// Box-box tests
	// Intersecting
	Box bb1{ 0, 3, 3, 4, 4 };
	Box bb2{ 0, 5, 5, 4, 4 };
	std::cout << bb1.to_string() << " and " << bb2.to_string()
		<< " collision: " << box_box_intersect(bb1, bb2) << std::endl;

	// Non-Intersecting
	Box bb3{ 0, 2, 2, 2, 2 };
	Box bb4{ 0, 8, 8, 2, 2 };
	std::cout << bb1.to_string() << " and " << bb2.to_string()
		<< " collision: " << box_box_intersect(bb3, bb4) << std::endl;


	// Box-circle tests
	// Intersecting
	Box box1{ 0, 5, 5, 4, 4 };
	Circle circle1{ 0, 5, 5, 3 };
	std::cout << box1.to_string() << " and " << circle1.to_string()
		<< " collision: " << circle_box_intersect(circle1, box1) << std::endl;

	// Non-Intersecting
	Box box2{ 0, 2, 2, 2, 2 };
	Circle circle2{ 0, 8, 8, 1 };
	std::cout << box2.to_string() << " and " << circle2.to_string()
		<< " collision: " << circle_box_intersect(circle2, box2) << std::endl;


	// Circle-circle tests
	// Intersecting
	Circle cc1{ 0, 3, 3, 4 };
	Circle cc2{ 0, 5, 5, 4 };
	std::cout << cc1.to_string() << " and " << cc2.to_string()
		<< " collision: " << circle_circle_intersect(cc1, cc2) << std::endl;

	// Non-Intersecting
	Circle cc3{ 0, 2, 2, 2 };
	Circle cc4{ 0, 8, 8, 1 };
	std::cout << cc3.to_string() << " and " << cc4.to_string()
		<< " collision: " << circle_circle_intersect(cc3, cc4) << std::endl;
	

}
