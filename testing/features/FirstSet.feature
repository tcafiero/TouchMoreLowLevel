Feature: EmptyProject
	Scenario Outline: Sum
		Given the two integer <A> <B>
		When you make the sum
		Then the output should be <Sum>

		Examples:
		| A | B | Sum |
		| 3 | 5 | 8 |
		| 5 | 5 | 10 |

	Scenario Outline: Sub
		Given the two integer <A> <B>
		When you make the sub
		Then the output should be <Diff>


		Examples:
		| A | B | Diff |
		| 5 | 3 | 2 |
		| 5 | 4 | 1 |

	Scenario Outline: End
		Given Operation <Oper>
		Then Flush Data	

		Examples:
		| Oper |
		| End |
		
	