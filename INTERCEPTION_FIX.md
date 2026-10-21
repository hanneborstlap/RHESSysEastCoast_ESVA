# Customized BirchBranch interception fix

This copy preserves the customized solar radiation and ditch code and corrects
canopy storage accounting, repeated mutating calls, and water allocation to
solar/open strata at the same canopy height.

Build inside this directory with:
`make PGM=rhessysEC.7.2-birch-interception-fix`

Run compilation and model computations through Slurm or a standard allocation.
Read `../../../notes/birchbranch_interception_fix.md` for assumptions,
equations, regression results, reproduction commands, and remaining basin work.
The corrections have focused regression tests; basin closure is not yet proven.
