# medida ordinal
convert mo_averages.png mo_sorted.png -splice 10x0 -background "#ffffff" +append -crop +10x-0 mo_fi
nal.png

# rbp
convert rbp_total.png rbp_ring_1.png rbp_ring_2.png rbp_ring_3.png rbp_ring_4.png -splice 10x0 -background "#ffffff" +append -crop +10x-0 brp_aneis.png
convert rbp_total.png rbp_slice.png -splice 100x0 -background "#ffffff" -gravity bottom +append -crop +100x-0 brp_fatia.png

# framediff
convert frame_diff_1.png frame_diff_2.png -splice 10x0 -background "#ffffff" +append -crop +10x-0 frame_diff_intermediario.png
convert frame_diff_bw.png frame_diff_colored.png -splice 10x0 -background "#ffffff" +append -crop +10x-0 frame_diff_intermediario2.png
convert frame_diff_intermediario.png frame_diff_intermediario2.png -splice 0x10 -background "#ffffff" +append -crop -0x+10 frame_diff_final.png
rm frame_diff_intermediario.png
rm frame_diff_intermediario2.png