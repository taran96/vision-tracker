for file in visiontracker/test/*; do
  if [[ "$file" != *"runtests.sh" ]]; then
    [ -f "$file" ] && [ -x "$file" ] && "$file"
  fi
done
